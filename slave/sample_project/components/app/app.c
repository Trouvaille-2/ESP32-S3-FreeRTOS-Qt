#include "app.h"
#include "myuart.h"
#include "modbus.h"
#include "device.h"   // 寄存器数据中心
#include "pid.h"      // PID算法库
#include "motor.h"    // 电机驱动与仿真
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "APP";
/* 1. 定义两个 PID 控制器对象（双环核心！） */
static PID_t s_pos_pid; // 位置外环
static PID_t s_spd_pid; // 速度内环

static void modbus_task(void *arg)
{
    uint8_t buf[256];
    int idx = 0;
    uint32_t last = 0;

    while(1)
    {
        uint8_t byte;
        int n = uart_read(&byte, 1, 5);
        if(n > 0)
        {
            buf[idx++] = byte;
            last = xTaskGetTickCount();
        }
        else if(idx > 0 && (xTaskGetTickCount() - last) >= pdMS_TO_TICKS(5))
        {
            uint8_t rep[256];
            int len = modbus_slave_process(buf, idx, rep);
            if (len > 0)
            {
                uart_write(rep, len);
            }
            idx = 0;
        }
    }
}

static void control_task(void *arg)
{
    TickType_t last_wake_time = xTaskGetTickCount();
    const TickType_t period_ticks = pdMS_TO_TICKS(10); // 10ms控制心跳
    const float dt = 0.01f; // 10ms (0.01s)

    while(1)
    {
        // 1. 绝对精准定点唤醒
        vTaskDelayUntil(&last_wake_time, period_ticks);

        // 2. 读当前控制指令与运行模式
        uint16_t cmd  = device_reg_read(REG_CTRL_CMD);  // 控制字：0=停止，1=使能，2=刹车，3=回零
        uint16_t mode = device_reg_read(REG_CTRL_MODE); // 模式：0=开环，1=单速度，2=双环

        // 3. 处理回零指令
        if(cmd == 3)
        {
            motor_clear_encoder();
            device_reg_write(REG_CTRL_CMD, 0); // 搞定后回到停止状态
            cmd = 0;
        }

        // 4. 处理紧急刹车
        if(cmd == 2)
        {
            motor_brake();
            PID_Reset(&s_pos_pid);
            PID_Reset(&s_spd_pid);
            continue;
        }

        // 5. 处理正常停止
        if(cmd == 0)
        {
            motor_stop();
            PID_Reset(&s_pos_pid);
            PID_Reset(&s_spd_pid);
            // 顺便把实际转速和脉冲更新到寄存器，保证停止时示波器能看到当前静止状态
            float cur_spd = motor_get_speed_rpm(dt);
            int32_t cur_pos = motor_get_encoder_pulse();
            device_reg_write_int32(REG_ACTUAL_POS_H, cur_pos);
            device_reg_write(REG_ACTUAL_SPD, (int16_t)cur_spd);
            device_reg_write(REG_PWM_OUTPUT, 0);
            continue;
        }

        /* 核心运行状态 (cmd == 1: 使能运行) */

        // 采集当前电机的实际物理状态
        float actual_spd   = motor_get_speed_rpm(dt);    // 当前实际转速 (RPM)
        int32_t actual_pos = motor_get_encoder_pulse();  // 当前实际位置脉冲

        // 实时从寄存器同步 PID 参数（支持上位机在线调参）
        // 位置外环参数：
        s_pos_pid.Kp       = (float)device_reg_read(REG_POS_KP) / 100.0f; // 比如 200 -> 2.00
        s_pos_pid.Ki       = (float)device_reg_read(REG_POS_KI) / 100.0f;
        s_pos_pid.Kd       = (float)device_reg_read(REG_POS_KD) / 100.0f;
        s_pos_pid.out_max  = (float)device_reg_read(REG_POS_MAX_SPD);     // 外环最大限速 (比如 300 RPM)
        s_pos_pid.out_min  = -s_pos_pid.out_max;
        s_pos_pid.deadband = (float)device_reg_read(REG_POS_DEADBAND);    // 定位死区 (比如 5 个脉冲)

        // 速度内环参数：
        s_spd_pid.Kp           = (float)device_reg_read(REG_SPD_KP) / 100.0f;
        s_spd_pid.Ki           = (float)device_reg_read(REG_SPD_KI) / 100.0f;
        s_spd_pid.Kd           = (float)device_reg_read(REG_SPD_KD) / 100.0f;
        s_spd_pid.out_max      = (float)device_reg_read(REG_SPD_MAX_PWM) / 10.0f; // 1000‰ -> 100.0%
        s_spd_pid.out_min      = -s_spd_pid.out_max;
        s_spd_pid.max_integral = (float)device_reg_read(REG_SPD_MAX_I) / 10.0f;   // 抗积分饱和
        s_spd_pid.min_integral = -s_spd_pid.max_integral;

        // 准备目标变量和输出变量
        float output_duty = 0.0f; // 最终输出给电机的占空比
        float target_spd  = 0.0f; // 目标转速
        int32_t target_pos = 0;
        
        /* 模式 0：开环占空比模式 */
        if(mode == 0)
        {
            // 直接读取上位机下发的固定油门
            output_duty = (float)(int16_t)device_reg_read(REG_PWM_OUTPUT) / 10.0f; // 1000‰ -> 100.0%
        }
        /* 模式 1：单速度闭环模式 */
        else if(mode == 1)
        {
            // 读取上位机下发的目标转速
            target_spd = (float)(int16_t)device_reg_read(REG_TARGET_SPD);
            // 速度环计算输出占空比
            output_duty = PID_update(&s_spd_pid, actual_spd, dt, target_spd);
        }
        /* 模式 2：位置-速度双环串级闭环模式 */
        else if(mode == 2)
        {
            // 读取 32 位目标位置脉冲 (比如 50000 脉冲)
            target_pos = device_reg_read_int32(REG_TARGET_POS_H);
            // ① 位置外环计算：根据位置误差，算出期望转速
            target_spd = PID_update(&s_pos_pid, (float)actual_pos, dt, (float)target_pos);
            // ② 速度内环计算：速度环紧紧咬住外环给出的转速，算出最终 PWM 油门！
            output_duty = PID_update(&s_spd_pid, actual_spd, dt, target_spd);

            // 将外环算出的期望转速和跟踪误差存回寄存器，供 Qt 示波器画曲线！
            device_reg_write(REG_TARGET_SPD, (int16_t)target_spd);
            device_reg_write(REG_POS_ERROR, (int16_t)(target_pos - actual_pos));
        }

        // 驱动电机执行器
        motor_set_output(output_duty);

        // 将当前电机的最新真实状态回写到寄存器
        device_reg_write_int32(REG_ACTUAL_POS_H, actual_pos);
        device_reg_write(REG_ACTUAL_SPD, (int16_t)actual_spd);
        device_reg_write(REG_PWM_OUTPUT, (int16_t)(output_duty * 10.0f)); // 100.0% -> 1000‰
    }
}

void app_init(void)
{
    // 1. 初始化 48 个寄存器数据中心
    device_init_regs();

    // 2. 初始化串口并启动 Modbus 任务
    uart_init();
    xTaskCreate(modbus_task, "modbus", 4096, NULL, 5, NULL);

    // 3. 配置电机参数
    motor_config_t mcfg = {
        .pwm_pin   = 15, // PWMA 引脚
        .in1_pin   = 16, // AIN1 引脚
        .in2_pin   = 17, // AIN2 引脚
        .stby_pin  = 18, // STBY 待机引脚
        .enc_a_pin = 4,  // 编码器 A 相
        .enc_b_pin = 5,  // 编码器 B 相
        .ppr       = 330,// 减速后输出轴每转总脉冲 330
        .simulation_mode = true, // ✅ 软件在环仿真开关开启！
    };
    motor_init(&mcfg);

    // 4. 启动 100Hz (10ms) 周期双环控制任务 (优先级 6，确保高实时性)
    xTaskCreate(control_task, "control_task", 4096, NULL, 6, NULL);
    ESP_LOGI(TAG, "🚀 直流伺服系统初始化完成！[双环控制 100Hz + Modbus 通信在线]");
}