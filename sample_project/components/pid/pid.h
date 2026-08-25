#ifndef __PID_H
#define __PID_H

#include <stdint.h>
#include <stdbool.h>

typedef struct{
    float Kp;
    float Ki;
    float Kd;
    float setpoint;
    float feedback;
    float error;
    float last_error;
    float integral;
    float derivative;
    float out_min, out_max;
    float max_integral; // 积分抗饱和上限值
    float min_integral; // 积分抗饱和下限值
    float deadband; // 死区
}PID_t;

void PID_Init(PID_t *p, float Kp, float Ki, float Kd, float setpoint, float out_min, float out_max, float max_integral, float min_integral, float deadband);

void PID_Reset(PID_t *p);

void PID_SetGains(PID_t *p, float Kp, float Ki, float Kd);

float PID_update(PID_t *p, float feedback, float dt, float setpoint);

#endif