#include "myuart.h"
#include "driver/uart.h"   
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"

void uart_init()
{
    uart_config_t uart_config = {
        .baud_rate = 9600,//波特率
        .data_bits = UART_DATA_8_BITS,//数据位宽度
        .parity    = UART_PARITY_DISABLE,//奇偶校验位
        .stop_bits = UART_STOP_BITS_1,//停止位
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,//是否启用rts/cts硬件流控
        .source_clk = UART_SCLK_DEFAULT,//时钟源
        .rx_flow_ctrl_thresh = 100//流控阈值
    };

    // 1. 将 UART 配置结构体的参数写入硬件寄存器
//    UART_NUM_1：选择 UART1 外设（对应硬件 UART1）
//    &uart_config：指向之前填充好的 uart_config_t 结构体（波特率、数据位等）
    uart_param_config(UART_NUM_1, &uart_config);

// 2. 将 UART 的 TX/RX 功能映射到具体的 GPIO 引脚
//    UART_NUM_1：同上，指定 UART1
//    GPIO_NUM_17：TX 引脚（发送）
//    GPIO_NUM_18：RX 引脚（接收）
//    后面两个 -1 分别表示 RTS（请求发送）和 CTS（清除发送）引脚，-1 表示不使用硬件流控
    uart_set_pin(UART_NUM_1, GPIO_NUM_17, GPIO_NUM_18, -1, -1);

// 3. 安装 UART 驱动程序，分配缓冲区并启动硬件
//    UART_NUM_1：指定 UART1
//    第一个 1024：接收缓冲区大小（字节）
//    第二个 1024：发送缓冲区大小（字节）
//    0：事件队列大小（0 表示不使用事件队列）
//    NULL：事件队列句柄（未使用，传 NULL）
//    0：分配标志（通常传 0）
    uart_driver_install(UART_NUM_1, 1024, 1024, 0, NULL, 0);
}

int uart_read(uint8_t *buf,int len,int timeout_ms)
{
    return uart_read_bytes(MODBUS_UART_NUM, buf, len, pdMS_TO_TICKS(timeout_ms));
}

int uart_write(const uint8_t *buf, int len)
{
    return uart_write_bytes(MODBUS_UART_NUM, (const char *)buf, len);
}
