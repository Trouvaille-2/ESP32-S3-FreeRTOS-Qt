#include "modbus.h"
#include <stdint.h>
#include "device.h"

uint16_t modbus_crc16(const uint8_t *data, size_t length)
{
    uint16_t crc = 0xFFFF;
    for (size_t i = 0; i < length; i++)
    {
        crc ^= data[i];
        for (int j = 0; j < 8; j++)
        {
            if (crc & 0x0001)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
    }
    return crc;
}

// 处理从站请求
static int append_crc(uint8_t *resp, int len)
{
    uint16_t crc = modbus_crc16(resp,len);
    resp[len++] = crc & 0xFF;
    resp[len++] = (crc >> 8) & 0xFF;
    return len;
}

//异常应答,当设备返回"出错了"时，功能码最高位置 1
static int modbus_exception(uint8_t func, uint8_t code,uint8_t *resp)
{
    int len=0;
    resp[len++]=SLAVE_ADDRESS;
    resp[len++]=func|0x80;
    resp[len++]=code;
    return append_crc(resp,len);
    //0x01 非法功能码
    //0x02 非法数据地址
    //0x03 非法数据值
}

int modbus_slave_process(const uint8_t *frame,int len,uint8_t *resp)
{
    // 合法性检验
    if(len<4) return 0;
    if(frame[0] != SLAVE_ADDRESS) return 0;
    uint16_t rx_crc = (uint16_t)(frame[len-1] << 8) | frame[len-2];
    if(modbus_crc16(frame, len-2) != rx_crc) return 0;
    
    // 处理功能码
    switch(frame[1])
    {
        case 0x03:
        {
            uint16_t start = ((uint16_t)frame[2] << 8) | frame[3];
            uint16_t qty   = ((uint16_t)frame[4] << 8) | frame[5];
            if (qty == 0 || qty > 125)  return modbus_exception(0x03, 0x03, resp);
            if (start + qty > NUM_REGS) return modbus_exception(0x03, 0x02, resp);
            int len = 0;
            resp[len++] = SLAVE_ADDRESS;
            resp[len++] = 0x03;
            resp[len++] = qty * 2;          /* 字节数 */
            for (uint16_t i = 0; i < qty; i++)
            {
                uint16_t v = device_reg_read(start + i);
                resp[len++] = v >> 8;       /* 高字节在前 */
                resp[len++] = v & 0xFF;
            }
            return append_crc(resp, len);
        }
            
        case 0x06:
            {
                uint16_t addr = ((uint16_t)frame[2] << 8) | frame[3];
                uint16_t val  = ((uint16_t)frame[4] << 8) | frame[5];
                if (addr >= NUM_REGS) return modbus_exception(0x06, 0x02, resp);
                device_reg_write(addr, val);
                /* 06 应答 = 原样回显整帧请求（去掉 CRC 后重新 append） */
                for (int i = 0; i < len - 2; i++) resp[i] = frame[i];
                return append_crc(resp, len - 2);
            }
        case 0x10:
            {
                uint16_t addr=((uint16_t) frame[2] << 8) | frame[3];
                uint16_t qty =((uint16_t) frame[4] << 8) | frame[5];
                if (qty == 0 || qty > 123) return modbus_exception(0x10, 0x03, resp);
                if (addr + qty > NUM_REGS) return modbus_exception(0x10, 0x02, resp);
                uint8_t byte_count=frame[6];
                if(byte_count != qty*2) return modbus_exception(0x10, 0x03, resp);
                if(len != 7+byte_count+2) return modbus_exception(0x10, 0x03, resp);
                for(uint16_t i=0;i<qty;i++)
                {
                    uint16_t val=((uint16_t) frame[7+i*2] << 8) | frame[8+i*2];
                    device_reg_write(addr+i,val);
                }
                int rlen=0;
                resp[rlen++]=SLAVE_ADDRESS;
                resp[rlen++]=0x10;
                resp[rlen++]=(uint8_t)(addr >> 8);  /* 起始地址高字节 */
                resp[rlen++]=(uint8_t)(addr & 0xFF);  /* 起始地址低字节 */
                resp[rlen++]=(uint8_t)(qty >> 8);  /* 寄存器数量高字节 */
                resp[rlen++]=(uint8_t)(qty & 0xFF);  /* 寄存器数量低字节 */
                return append_crc(resp,rlen);
        default:
            return modbus_exception(frame[1], 0x01, resp);  /* 非法功能码 */
        }
    }
}

