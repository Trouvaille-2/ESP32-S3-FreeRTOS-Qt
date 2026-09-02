#ifndef __MOTOR_H
#define __MOTOR_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

typedef struct {
    int pwm_pin;            /* PWMA: PWM 调速引脚  */
    int in1_pin;            /* AIN1: 方向控制 1  */
    int in2_pin;            /* AIN2: 方向控制 2  */
    int stby_pin;           /* STBY: 使能引脚*/
    int enc_a_pin;          /* 编码器 A 相 */
    int enc_b_pin;          /* 编码器 B 相  */
    int ppr;                /* 编码器每转总脉冲数  */
    bool simulation_mode;   /* 是否开启软件仿真模式  */
} motor_config_t;

esp_err_t motor_init(const motor_config_t *config);

void motor_set_output(float duty_percent);

int32_t motor_get_encoder_pulse(void);

float motor_get_speed_rpm(float dt);

void motor_stop(void);

void motor_brake(void);

void motor_clear_encoder(void);

#endif
