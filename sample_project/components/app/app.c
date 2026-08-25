#include "app.h"
#include "myuart.h"
#include "modbus.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static void modbus_task(void *arg)
{
    uint8_t buf[256];
    int idx = 0;
    uint32_t last = 0;

    while(1)
    {
        uint8_t byte;
        int n=uart_read(&byte,1,5);
        if(n>0)
        {
            buf[idx++] = byte;
            last = xTaskGetTickCount();
        }
        else if(idx>0 && (xTaskGetTickCount()-last)>=pdMS_TO_TICKS(5))
        {
            uint8_t rep[256];
            int len = modbus_process(buf,idx,rep);
            if (len>0)
            {
                uart_write(rep,len);
            }
            idx = 0;
        }
    }
}

void app_init()
{
    uart_init();
    xTaskCreate(modbus_task,"modbus",4096,NULL,5,NULL);
}


