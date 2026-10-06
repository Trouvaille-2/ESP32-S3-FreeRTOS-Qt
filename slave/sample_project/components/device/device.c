#include "device.h"
#include <stdint.h>
#include <string.h>

static uint16_t device_regs[NUM_REGS];

void device_init_regs(void)
{
    memset(device_regs, 0, sizeof(device_regs));

    /* 初始化寄存器默认值 */
    device_regs[REG_CTRL_CMD] = 0; /* 默认停止 */
    device_regs[REG_CTRL_MODE] = 2; /* 默认双环 */
    device_regs[REG_SYS_STATUS] = 0; 
    device_regs[REG_ERR_CODE] = 0; /* 默认正常 */

    device_reg_write_int32(REG_TARGET_POS_H, 0); /* 默认目标位置为 0 */

    /* 初始化 外环PID 参数默认值 */
    device_regs[REG_POS_KP] = 200; /* 位置环 Kp = 2.00 */
    device_regs[REG_POS_KI] = 5;   /* 位置环 Ki = 0.05 */
    device_regs[REG_POS_KD] = 10;  /* 位置环 Kd = 0.10 */   
    device_regs[REG_POS_MAX_SPD] = 300; /* 位置环最大输出限速 = 300 RPM */
    device_regs[REG_POS_DEADBAND] = 5; /* 定位死区范围 = 5 脉冲 */

    /* 初始化内环环 PID 参数默认值 */
    device_regs[REG_SPD_KP] = 150; /* 速度环 Kp = 1.50 */
    device_regs[REG_SPD_KI] = 20;  /* 速度环 Ki = 0.20 */
    device_regs[REG_SPD_KD] = 5;   /* 速度环 Kd = 0.05 */
    device_regs[REG_SPD_MAX_PWM] = 1000; /* 100%占空比 */
    device_regs[REG_SPD_MAX_I] = 500; /* 速度环抗积分饱和限幅 = 50.0 ‰ */

}

/* 16 位寄存器读写 (供 Modbus 引擎直接调用) */
uint16_t device_reg_read(int reg)
{
    if (reg >= 0 && reg < NUM_REGS)
    {
        return device_regs[reg];
    }
    return 0;
}
void device_reg_write(int reg, uint16_t val)
{
    if (reg >= 0 && reg < NUM_REGS)
    {
        device_regs[reg] = val;
    }
}

/* 32 位整型 (位置脉冲) 便捷读写辅助函数 */
int32_t device_reg_read_int32(int reg_high)
{
    if(reg_high>=0 && (reg_high+1)<NUM_REGS)
    {
        uint16_t h = device_regs[reg_high];
        uint16_t l = device_regs[reg_high+1];
        return (int32_t)((((uint32_t)h)<<16) | l);
    }
    return 0;
}
void device_reg_write_int32(int reg_high, int32_t val)
{
    if(reg_high>=0 && (reg_high+1)<NUM_REGS)
    {
        device_regs[reg_high]=(val>>16)&0xFFFF;
        device_regs[reg_high+1]=val&0xFFFF;
    }
}

