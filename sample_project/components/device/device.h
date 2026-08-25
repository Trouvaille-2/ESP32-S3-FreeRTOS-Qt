#ifndef DEVICE_H
#define DEVICE_H
#include <stdint.h>

#define NUM_REGS 48 /* 寄存器总数量 (覆盖 0x0000 ~ 0x0024，预留到 48) */

/* ==================== 1. 系统控制与状态区 (0x0000 ~ 0x0003) ==================== */
#define REG_CTRL_CMD 0x0000 /* 控制字: 0-停止, 1-使能运行, 2-刹车, 3-回零 */
#define REG_CTRL_MODE 0x0001 /* 模式: 0-开环, 1-单速度环, 2-双环(位置+速度) */
#define REG_SYS_STATUS 0x0002 /* 状态字: Bit0-使能, Bit1-到位, Bit2-报警 */
#define REG_ERR_CODE 0x0003 /* 故障码: 0-正常, 1-堵转, 2-超速, 3-通信超时 */

/* ==================== 2. 实时运动数据与示波器采样区 (0x0004 ~ 0x000B) ==================== */
#define REG_TARGET_POS_H      0x0004   /* 目标位置 高16位 (脉冲) */
#define REG_TARGET_POS_L      0x0005   /* 目标位置 低16位 (脉冲) */
#define REG_ACTUAL_POS_H      0x0006   /* 实际位置 高16位 (编码器反馈) */
#define REG_ACTUAL_POS_L      0x0007   /* 实际位置 低16位 (编码器反馈) */
#define REG_TARGET_SPD        0x0008   /* 期望速度 (RPM, 双环下由位置环输出) */
#define REG_ACTUAL_SPD        0x0009   /* 实际速度 (RPM, 编码器测速) */
#define REG_PWM_OUTPUT        0x000A   /* 最终输出占空比 (-1000 ~ +1000 对应 -100.0% ~ +100.0%) */
#define REG_POS_ERROR         0x000B   /* 当前位置跟踪误差 (脉冲数) */

/* ==================== 3. 位置外环 PID 参数区 (0x0010 ~ 0x0014) ==================== */
#define REG_POS_KP            0x0010   /* 位置环 Kp * 100 (如 200 表示 2.00) */
#define REG_POS_KI            0x0011   /* 位置环 Ki * 100 (如 5 表示 0.05) */
#define REG_POS_KD            0x0012   /* 位置环 Kd * 100 (如 10 表示 0.10) */
#define REG_POS_MAX_SPD       0x0013   /* 位置环最大输出限速 (RPM, 默认 300) */
#define REG_POS_DEADBAND      0x0014   /* 定位死区范围 (脉冲, 默认 5) */
/* ==================== 4. 速度内环 PID 参数区 (0x0020 ~ 0x0024) ==================== */
#define REG_SPD_KP            0x0020   /* 速度环 Kp * 100 (如 150 表示 1.50) */
#define REG_SPD_KI            0x0021   /* 速度环 Ki * 100 (如 20 表示 0.20) */
#define REG_SPD_KD            0x0022   /* 速度环 Kd * 100 (如 5 表示 0.05) */
#define REG_SPD_MAX_PWM       0x0023   /* 速度环最大输出限幅 (‰, 默认 1000) */
#define REG_SPD_MAX_I         0x0024   /* 速度环抗积分饱和限幅 (‰, 默认 500) */

/* ==================== 函数接口 ==================== */
void device_init_regs(void);/* 初始化寄存器默认值 */
uint16_t device_reg_read(int reg);/* 标准 16 位寄存器读写 (供 Modbus 引擎直接调用) */
void device_reg_write(int reg, uint16_t val);



int32_t device_reg_read_int32(int reg_high);
void device_reg_write_int32(int reg_high, int32_t val);/* 32 位整型 (位置脉冲) 便捷读写辅助函数 */

#endif