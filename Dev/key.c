#include "ch32v30x.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "key.h"
#include "queue.h"

#define KEY_SCAN_PERIOD_MS 20U

static volatile uint8_t key_state;
static uint8_t last_sample;

QueueHandle_t key_queue;

static uint8_t KEY_ReadRaw(void)
{
    return (uint8_t)(~(GPIO_ReadInputData(GPIOE) >> 1)) & KEY_ALL;
}

static void KEY_TimerCallback(TimerHandle_t timer)
{
    uint8_t sample;
    uint8_t pressed;

    (void)timer;
    sample = KEY_ReadRaw();

    if(sample == last_sample)
    {
        pressed = sample & (uint8_t)~key_state;
        key_state = sample;
        if(pressed != 0U && key_queue != NULL)
        {
            xQueueSend(key_queue, &pressed, 0);
        }
    }
    else
    {
        last_sample = sample;
    }
}

void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TimerHandle_t timer;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
                                  GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    key_state = 0U;
    last_sample = KEY_ReadRaw();

    timer = xTimerCreate("KeyScan",
                         pdMS_TO_TICKS(KEY_SCAN_PERIOD_MS),
                         pdTRUE,
                         NULL,
                         KEY_TimerCallback);
    configASSERT(timer != NULL);
    configASSERT(xTimerStart(timer, 0) == pdPASS);
}

uint8_t KEY_GetState(void)
{
    return key_state;
}
