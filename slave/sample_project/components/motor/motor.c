#include "motor.h"
#include <stdio.h>
#include <math.h>
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "driver/pulse_cnt.h"
#include "esp_log.h"

static const char *TAG = "MOTOR";

static motor_config_t s_cfg;//保存当前电机的引脚与模式配置。

static pcnt_unit_handle_t s_pcnt_unit = NULL;//硬件编码器

static int32_t s_accumulated_pulse = 0;//32为累计脉冲
static int32_t s_last_raw_count = 0;//上一次读到的硬件计数

/* 3. 软件仿真状态变量 (仿真模式用) */
static float s_sim_speed_rpm = 0.0f;    // 仿真当前转速
static float s_applied_duty = 0.0f;     // 记录当前下发的占空比

void motor_set_output(float duty_percent)
{
    if(duty_percent>100.0f)
    {
        duty_percent=100.0f;
    }
    else if(duty_percent<-100.0f)
    {
        duty_percent=-100.0f;
    }

    if(s_cfg.simulation_mode)
    {
        s_applied_duty = duty_percent;//记下当前油门，供仿真模型算转速
        return;
    }

    uint32_t duty_raw = (uint32_t)(fabsf(duty_percent)/100.0f*1023.0f);

    if(duty_percent>0.5f)
    {
        //正转
        gpio_set_level(s_cfg.in1_pin,1);
        gpio_set_level(s_cfg.in2_pin,0);
    }
    else if(duty_percent<-0.5f)
    {
        //反转
        gpio_set_level(s_cfg.in1_pin,0);
        gpio_set_level(s_cfg.in2_pin,1);
    }
    else
    {
        //不转
        gpio_set_level(s_cfg.in1_pin,0);
        gpio_set_level(s_cfg.in2_pin,0);
    }

    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty_raw);//防止 PWM 波形“被撕裂产生硬件毛刺,把新的占空比数值写到后台缓冲区中
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}

void motor_stop(void)//自由停止，不伤机器
{
    motor_set_output(0.0f);
}

void motor_brake(void)//应对特殊情况
{
    motor_set_output(0.00f);
    if(!s_cfg.simulation_mode)
    {
    gpio_set_level(s_cfg.in1_pin,1);
    gpio_set_level(s_cfg.in2_pin,1);
    }
}


float motor_get_speed_rpm(float dt)//接受控制周期，返回当前电机的实时转速
{
    if(dt <= 0.0001f)//安全保护，防止除以0；
    {
        dt=0.01f;
    }

    /* 1. 软件仿真模式 */
    if(s_cfg.simulation_mode)
    {
        const float max_rpm = 330.0f; // 空载最大转速330RPM
        const float tau = 0.12f; //机械惯性时间常数0.12秒

        float target_rpm = (s_applied_duty / 100.0f) * max_rpm;

        // 简单一阶惯性模型
        s_sim_speed_rpm += (target_rpm - s_sim_speed_rpm) * (dt / (tau + dt));

        // 积分出累计脉冲
        s_accumulated_pulse += (int32_t)(s_sim_speed_rpm * s_cfg.ppr / 60.0f * dt);
        
        return s_sim_speed_rpm;
    }

    /* 2. 硬件编码器模式 */
    if(s_pcnt_unit == NULL)
    {
        return 0.0f;
    }

    int cur_count = 0;
    pcnt_unit_get_count(s_pcnt_unit, &cur_count);//读硬件计数器
    int delta = cur_count - s_last_raw_count;//本周期脉冲增量
    s_last_raw_count = cur_count;
    s_accumulated_pulse += delta; //累加总脉冲

    float rpm = (float)delta / (float)s_cfg.ppr * 60.0f / dt; //计算转速
    return rpm;
}

//获取当前电机的32位累计位置脉冲
int32_t motor_get_encoder_pulse(void)
{
    return s_accumulated_pulse;
}

//编码器回零（机械原点复位）
void motor_clear_encoder(void)
{
    s_accumulated_pulse = 0;
    s_last_raw_count = 0;

    //底层的PCNT硬件计数器也清零
    if(!s_cfg.simulation_mode && s_pcnt_unit != NULL)
    {
        pcnt_unit_clear_count(s_pcnt_unit);
    }
}


esp_err_t motor_init(const motor_config_t *config)
{
    if(config==NULL)
    {
        ESP_LOGE(TAG,"motor_init: config is NULL");
        return ESP_ERR_INVALID_ARG;//空指针安全防护
    }

    //保存配置参数到全局结构体
    s_cfg = *config;
    s_accumulated_pulse = 0;
    s_last_raw_count = 0;
    s_sim_speed_rpm = 0.0f;
    s_applied_duty = 0.0f;

    //如果开启了纯软件仿真模式，则不初始化硬件
    if(s_cfg.simulation_mode)
    {
        ESP_LOGW(TAG,"motor_init: simulation mode enabled, hardware not initialized");
        return ESP_OK;
    }

    /*真实硬件外设初始化*/

    gpio_config_t io_conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << s_cfg.in1_pin) | (1ULL << s_cfg.in2_pin),
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    if(s_cfg.stby_pin >= 0)
    {
        io_conf.pin_bit_mask |= (1ULL << s_cfg.stby_pin);
    }
    gpio_config(&io_conf);

    //初始状态：两路方向置0，STBY置1（唤醒TB6612）
    gpio_set_level(s_cfg.in1_pin, 0);
    gpio_set_level(s_cfg.in2_pin, 0);
    if (s_cfg.stby_pin >= 0) {
        gpio_set_level(s_cfg.stby_pin, 1);
    }

    //配置LEDC PWM定时器
    ledc_timer_config_t timer_conf = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_10_BIT, // 10位分辨率 (0 ~ 1023)
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 20000,                     // 20kHz 超静音高频 PWM
        .clk_cfg = LEDC_AUTO_CLK,
    };
    ledc_timer_config(&timer_conf);

    //配置LEDC PWM通道
    ledc_channel_config_t ch_conf = {
        .gpio_num = s_cfg.pwm_pin,            // 绑定到用户配置的 PWMA 引脚
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .intr_type = LEDC_INTR_DISABLE,
        .duty = 0,                            // 初始开机占空比为 0 (电机不转)
        .hpoint = 0,
    };
    ledc_channel_config(&ch_conf);

    //配置PCNT硬件编码器
    pcnt_unit_config_t unit_config = {
        .low_limit = -30000,
        .high_limit = 30000,
    };
    //创建计数单元句柄 s_pcnt_unit
    pcnt_new_unit(&unit_config, &s_pcnt_unit);

    //配置通道A
    pcnt_chan_config_t chan_a_config ={
        .edge_gpio_num = s_cfg.enc_a_pin,
        .level_gpio_num = s_cfg.enc_b_pin,
    };
    pcnt_channel_handle_t pcnt_chan_a=NULL;
    pcnt_new_channel(s_pcnt_unit, &chan_a_config, &pcnt_chan_a);

    //配置通道B
    pcnt_chan_config_t chan_b_config ={
        .edge_gpio_num = s_cfg.enc_b_pin,
        .level_gpio_num = s_cfg.enc_a_pin,
    };
    pcnt_channel_handle_t pcnt_chan_b=NULL;
    pcnt_new_channel(s_pcnt_unit, &chan_b_config, &pcnt_chan_b);

    //设置正交边沿动作
    pcnt_channel_set_edge_action(pcnt_chan_a, PCNT_CHANNEL_EDGE_ACTION_DECREASE, PCNT_CHANNEL_EDGE_ACTION_INCREASE);
    pcnt_channel_set_level_action(pcnt_chan_a, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE);
    pcnt_channel_set_edge_action(pcnt_chan_b, PCNT_CHANNEL_EDGE_ACTION_INCREASE, PCNT_CHANNEL_EDGE_ACTION_DECREASE);
    pcnt_channel_set_level_action(pcnt_chan_b, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE);

    //使能并启动计数器
    pcnt_unit_enable(s_pcnt_unit);
    pcnt_unit_clear_count(s_pcnt_unit);
    pcnt_unit_start(s_pcnt_unit);

    ESP_LOGI(TAG, "电机初始化成功：[真实硬件模式]，PWMA=%d, IN1=%d, IN2=%d", 
             s_cfg.pwm_pin, s_cfg.in1_pin, s_cfg.in2_pin);
    return ESP_OK; // 初始化完成！
}