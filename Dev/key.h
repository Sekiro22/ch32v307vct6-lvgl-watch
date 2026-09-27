#ifndef __KEY_H__
#define __KEY_H__

#include <stdint.h>
#include "queue.h"

#define KEY_PE1 (1U << 0)
#define KEY_PE2 (1U << 1)
#define KEY_PE3 (1U << 2)
#define KEY_PE4 (1U << 3)
#define KEY_PE5 (1U << 4)
#define KEY_PE6 (1U << 5)
#define KEY_ALL (KEY_PE1 | KEY_PE2 | KEY_PE3 | KEY_PE4 | KEY_PE5 | KEY_PE6)

extern QueueHandle_t key_queue;

void KEY_Init(void);
uint8_t KEY_GetState(void);

#endif
