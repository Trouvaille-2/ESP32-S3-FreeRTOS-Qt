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

    uart_param_config(MODBUS_UART_NUM, &uart_config);

    // 使用默认控制台引脚（免除外部引脚连接，直接走板载 Type-C USB 串口）
    uart_set_pin(MODBUS_UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, -1, -1);

    uart_driver_install(MODBUS_UART_NUM, 1024, 1024, 0, NULL, 0);
}

int uart_read(uint8_t *buf,int len,int timeout_ms)
{
    return uart_read_bytes(MODBUS_UART_NUM, buf, len, pdMS_TO_TICKS(timeout_ms));
}

int uart_write(const uint8_t *buf, int len)
{
    return uart_write_bytes(MODBUS_UART_NUM, (const char *)buf, len);
}
