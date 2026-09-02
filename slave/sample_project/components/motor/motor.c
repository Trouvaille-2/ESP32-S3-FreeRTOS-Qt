#include "motor.h"
#include <stdio.h>
#include <math.h>
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "driver/pulse_cnt.h"
#include "esp_log.h"

static const char *TAG = "MOTOR";

static motor_config_t s_cfg;//保存当前电机的引脚与模式配置。

static pcnt_uint_handle_t s_pcnt_unit = NULL;//硬件编码器

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


float motor_get_speed_rpm(float dt)
{
    
}
