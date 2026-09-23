#include "ch32v30x.h"
#include "debug.h"
#include "TFT.h"

static const TFT_InitCommand gc9a01_init_commands[] =
{
    {0xFE, 0, {0}, 0},
    {0xEF, 0, {0}, 0},
    {0xEB, 1, {0x14}, 0},
    {0x84, 1, {0x60}, 0},
    {0x85, 1, {0xFF}, 0},
    {0x86, 1, {0xFF}, 0},
    {0x87, 1, {0xFF}, 0},
    {0x88, 1, {0x0A}, 0},
    {0x89, 1, {0x23}, 0},
    {0x8A, 1, {0x00}, 0},
    {0x8B, 1, {0x80}, 0},
    {0x8C, 1, {0x01}, 0},
    {0x8D, 1, {0x03}, 0},
    {0x8E, 1, {0xFF}, 0},
    {0x8F, 1, {0xFF}, 0},

    {0x90, 4, {0x08, 0x08, 0x08, 0x08}, 0},
    {0xFF, 3, {0x60, 0x01, 0x04}, 0},

    {0xC3, 1, {0x13}, 0},
    {0xC4, 1, {0x13}, 0},
    {0xC9, 1, {0x30}, 0},

    {0xBE, 1, {0x11}, 0},
    {0xE1, 2, {0x10, 0x0E}, 0},
    {0xDF, 3, {0x21, 0x0C, 0x02}, 0},

    {0xF0, 6, {0x45, 0x09, 0x08, 0x08, 0x26, 0x2A}, 0},
    {0xF1, 6, {0x43, 0x70, 0x72, 0x36, 0x37, 0x6F}, 0},
    {0xF2, 6, {0x45, 0x09, 0x08, 0x08, 0x26, 0x2A}, 0},
    {0xF3, 6, {0x43, 0x70, 0x72, 0x36, 0x37, 0x6F}, 0},

    {0xED, 2, {0x1B, 0x0B}, 0},
    {0xAE, 1, {0x77}, 0},
    {0xCD, 1, {0x63}, 0},

    {0x70, 9, {0x07, 0x07, 0x04, 0x0E, 0x0F,
               0x09, 0x07, 0x08, 0x03}, 0},

    {0xE8, 1, {0x34}, 0},

    {0x60, 8, {0x38, 0x0B, 0x6D, 0x6D,
               0x39, 0xF0, 0x6D, 0x6D}, 0},

    {0x61, 8, {0x38, 0xF4, 0x6D, 0x6D,
               0x38, 0xF7, 0x6D, 0x6D}, 0},

    {0x62, 12, {0x38, 0x0D, 0x71, 0xED, 0x70, 0x70,
                0x38, 0x0F, 0x71, 0xEF, 0x70, 0x70}, 0},

    {0x63, 12, {0x38, 0x11, 0x71, 0xF1, 0x70, 0x70,
                0x38, 0x13, 0x71, 0xF3, 0x70, 0x70}, 0},

    {0x64, 7, {0x28, 0x29, 0xF1, 0x01,
               0xF1, 0x00, 0x07}, 0},

    {0x66, 10, {0x3C, 0x00, 0xCD, 0x67, 0x45,
                0x45, 0x10, 0x00, 0x00, 0x00}, 0},

    {0x67, 10, {0x00, 0x3C, 0x00, 0x00, 0x00,
                0x01, 0x54, 0x10, 0x32, 0x98}, 0},

    {0x74, 7, {0x10, 0x45, 0x80, 0x00,
               0x00, 0x4E, 0x00}, 0},

    {0x98, 2, {0x3E, 0x07}, 0},
    {0x99, 2, {0x3E, 0x07}, 0},
};

void TFT_SPI_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TFT_CS_HIGH();
    TFT_DC_DATA();
    TFT_RST_HIGH();

    SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init( SPI1, &SPI_InitStructure );

    SPI_Cmd( SPI1, ENABLE );
}

static void TFT_SPI_write(const uint8_t *data, uint32_t length)
{
    while(length --)
    {
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
        SPI_I2S_SendData(SPI1, *data++);
    }

    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
}

static void TFT_write_command(uint8_t command)
{
    TFT_CS_LOW();
    TFT_DC_CMD();

    TFT_SPI_write(&command, 1);

    TFT_CS_HIGH();
}

static void TFT_write_data(const uint8_t *data, uint32_t length)
{
    if(length == 0)return;

    TFT_CS_LOW();
    TFT_DC_DATA();

    TFT_SPI_write(data, length);

    TFT_CS_HIGH();
}

static void TFT_hardware_reset(void)
{
    TFT_RST_HIGH();
    Delay_Ms(10);

    TFT_RST_LOW();
    Delay_Ms(10);

    TFT_RST_HIGH();
    Delay_Ms(120);
}

void TFT_init(void)
{
    uint32_t i;
    uint8_t data;

    TFT_SPI_init();
    TFT_hardware_reset();

    //退出休眠
    TFT_write_command(0x11);
    Delay_Ms(120);

    //显存扫描方向
    data = 0x48;
    TFT_write_command(0x36);
    TFT_write_data(&data, 1);

    //RGB565
    data = 0x55;
    TFT_write_command(0x3A);
    TFT_write_data(&data, 1);

    for(i = 0; i < sizeof(gc9a01_init_commands) / sizeof(gc9a01_init_commands[0]); i ++)
    {
        TFT_write_command(gc9a01_init_commands[i].command);

        TFT_write_data(gc9a01_init_commands[i].data, gc9a01_init_commands[i].length);

        if(gc9a01_init_commands[i].delay_ms != 0)Delay_Ms(gc9a01_init_commands[i].delay_ms);
    }

    //开启颜色翻转
    TFT_write_command(0x21);

    //开启显示
    TFT_write_command(0x29);
    Delay_Ms(20);
}

void TFT_setWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
     uint8_t data[4];

    data[0] = x1 >> 8;
    data[1] = x1 & 0xFF;
    data[2] = x2 >> 8;
    data[3] = x2 & 0xFF;

    TFT_write_command(0x2A);
    TFT_write_data(data, 4);

    data[0] = y1 >> 8;
    data[1] = y1 & 0xFF;
    data[2] = y2 >> 8;
    data[3] = y2 & 0xFF;

    TFT_write_command(0x2B);
    TFT_write_data(data, 4);

    /* 开始写显存 */
    TFT_write_command(0x2C);
}

void TFT_writePixels(const uint16_t *pixels, uint32_t count)
{
    uint16_t color;

    TFT_CS_LOW();
    TFT_DC_DATA();

    while(count--)
    {
        color = *pixels++;

        while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
        SPI_I2S_SendData(SPI1, color >> 8);

        while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
        SPI_I2S_SendData(SPI1, color & 0xFF);
    }

    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);

    TFT_CS_HIGH();
}

void TFT_fillColor(uint16_t color)
{
    uint32_t count = TFT_WIDTH * TFT_HEIGHT;

    TFT_setWindow(0, 0, TFT_WIDTH - 1, TFT_HEIGHT - 1);

    TFT_CS_LOW();
    TFT_DC_DATA();

    while(count--)
    {
        while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
        SPI_I2S_SendData(SPI1, color >> 8);

        while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
        SPI_I2S_SendData(SPI1, color & 0xFF);
    }

    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);

    TFT_CS_HIGH();
}