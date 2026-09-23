#ifndef __TFT_H__
#define __TFT_H__

#include <stdint.h>

#define TFT_WIDTH   240
#define TFT_HEIGHT  240

#define TFT_RED     0xF800
#define TFT_GREEN   0x07E0
#define TFT_BLUE    0x001F
#define TFT_WHITE   0xFFFF
#define TFT_BLACK   0x0000 

#define TFT_CS_LOW()    GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define TFT_CS_HIGH()   GPIO_SetBits(GPIOA, GPIO_Pin_4)

#define TFT_DC_CMD()    GPIO_ResetBits(GPIOA, GPIO_Pin_3)
#define TFT_DC_DATA()   GPIO_SetBits(GPIOA, GPIO_Pin_3)

#define TFT_RST_LOW()   GPIO_ResetBits(GPIOA, GPIO_Pin_2)
#define TFT_RST_HIGH()  GPIO_SetBits(GPIOA, GPIO_Pin_2)

typedef struct
{
    uint8_t command;
    uint8_t length;
    uint8_t data[12];
    uint16_t delay_ms;
} TFT_InitCommand;

void TFT_SPI_init(void);
void TFT_init(void);
void TFT_setWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void TFT_writePixels(const uint16_t *pixels, uint32_t count);
void TFT_fillColor(uint16_t color);

#endif