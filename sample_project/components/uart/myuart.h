#ifndef __MYUART_H__
#define __MYUART_H__
#include "driver/uart.h"

#define MODBUS_UART_NUM UART_NUM_1

void uart_init();
int uart_read(uint8_t *buf,int len,int timeout_ms);
int uart_write(const uint8_t *buf, int len);

#endif