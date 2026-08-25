#include "pid.h"
#include <math.h>

void PID_Init(PID_t *p, float Kp, float Ki, float Kd, float setpoint, float out_min, float out_max, float max_integral, float min_integral, float deadband)
{
    p->Kp = Kp;
    p->Ki = Ki;
    p->Kd = Kd;
    p->out_min = out_min;
    p->out_max = out_max;

    if(max_integral <=0.001f)
    {
        max_integral = fabsf(out_max)*0.8f; // 积分抗饱和上限值默认设置为输出上限的 80%
    }else{
        max_integral = fabsf(max_integral);
    }
    p->max_integral = max_integral;
    p->min_integral = -p->max_integral;
    p->deadband = fabsf(deadband);

    PID_Reset(p);
}

void PID_Reset(PID_t *p)
{
    p->error = 0.0f;
    p->last_error = 0.0f;
    p->integral = 0.0f;
    p->derivative = 0.0f;
    p->setpoint = 0.0f;
    p->feedback = 0.0f;
}

void PID_SetGains(PID_t *p, float Kp, float Ki, float Kd)
{
    p->Kp = Kp;
    p->Ki = Ki;
    p->Kd = Kd;
}

float PID_update(PID_t *p, float feedback, float dt,float setpoint)
{
    if(dt<=0.0001f)
    {
        dt=0.01f;
    }
    p->feedback = feedback;
    p->setpoint=setpoint;
    p->error = p->setpoint - p->feedback;
    
    if(fabsf(p->error)<=p->deadband)
    {
        p->error = 0.0f;
        p->integral = 0.0f;   /* 进入死区后清除积分*/
    }

    float p_out=p->Kp * p->error;

    p->integral += p->error * dt;
     if (p->integral > p->max_integral) {
        p->integral = p->max_integral;
    } else if (p->integral < p->min_integral) {
        p->integral = p->min_integral;
    }
    float i_out=p->Ki*p->integral;

    p->derivative = (p->error - p->last_error) / dt;
    p->last_error = p->error;
    float d_out = p->Kd * p->derivative;

    float output=p_out+i_out+d_out;

    if (output > p->out_max) {
        output = p->out_max;
    } else if (output < p->out_min) {
        output = p->out_min;
    }

    return output;
}
