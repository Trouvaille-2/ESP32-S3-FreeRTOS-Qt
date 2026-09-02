#ifndef MODBUS_H_
#define MODBUS_H_
#include <stdint.h>
#include <stddef.h>

#define SLAVE_ADDRESS 0X01

uint16_t modbus_crc16(const uint8_t *data, size_t length);

int modbus_slave_process(const uint8_t *frame,int len,uint8_t *resp);



#endif