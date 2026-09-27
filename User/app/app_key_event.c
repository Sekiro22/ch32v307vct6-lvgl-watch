#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "key.h"
#include "debug.h"

TaskHandle_t key_taskhandle;

void key_event_task(void * key_event_arg)
{
    printf("key_event_task start!\n");
    uint8_t key_num;
    while(1)
    {
        if(xQueueReceive(key_queue, &key_num, portMAX_DELAY) == pdTRUE)
        {
            if(key_num & KEY_PE1)
            {
                printf("RET press\n");
            }
            if(key_num & KEY_PE2)
            {
                printf("SET press\n");
            }
            if(key_num & KEY_PE3)
            {
                printf("RIGHT press\n");
            }
            if(key_num & KEY_PE4)
            {
                printf("LETF press\n");
            }
            if(key_num & KEY_PE5)
            {
                printf("DOWN press\n");
            }
            if(key_num & KEY_PE6)
            {
                printf("UP press\n");
            }
        }
        else
        {
            printf("key_queue receiv error!\n");
        }
    }
}
