#include "TFT.h"
#include "FreeRTOS.h"
#include "task.h"
#include "debug.h"
#include "lvgl.h"
#include "ch32v307_gc9a01_ui.h"

#define LVGL_BUFFER_LINES 10

TaskHandle_t ui_taskhandle;

LV_ATTRIBUTE_MEM_ALIGN

static uint8_t lvgl_draw_buffer[
    TFT_WIDTH * LVGL_BUFFER_LINES * sizeof(uint16_t)
];
static lv_display_t * volatile flushing_display;

static uint32_t lvgl_tick_get(void)
{
    return (uint32_t)xTaskGetTickCount() * portTICK_PERIOD_MS;
}

static void lvgl_flush(lv_display_t *display,
                       const lv_area_t *area,
                       uint8_t *pixel_map)
{
    uint32_t width;
    uint32_t height;

    width = area->x2 - area->x1 + 1;
    height = area->y2 - area->y1 + 1;
    uint32_t pixel_count = width * height;

    TFT_setWindow(area->x1, area->y1, area->x2, area->y2);
    
    /* GC9A01 要求高字节先发送 */
    lv_draw_rgb565_swap(pixel_map, pixel_count);

    /* 必须在启动 DMA 前保存，避免 DMA 先完成 */
    flushing_display = display;

    TFT_writePixelsDMA(
        pixel_map,
        (uint16_t)(pixel_count * sizeof(uint16_t))
    );
}

void ui_thread(void * ui_arg)
{
    lv_display_t *display;
    printf("UI_Thread start!\n");

    lv_init();
    printf("lv_init OK!\n");
    lv_tick_set_cb(lvgl_tick_get);

    display = lv_display_create(TFT_WIDTH, TFT_HEIGHT);
    lv_display_set_color_format(display, LV_COLOR_FORMAT_RGB565);

    lv_display_set_buffers(display, lvgl_draw_buffer, NULL, sizeof(lvgl_draw_buffer), LV_DISPLAY_RENDER_MODE_PARTIAL);

    lv_display_set_flush_cb(display, lvgl_flush);
    printf("display init OK!\n");

    ch32v307_gc9a01_ui_init("");
    printf("UI_init OK!\n");

    lv_screen_load(screen_main_create());

    lv_obj_set_style_bg_color(
        lv_screen_active(),
        lv_color_hex(0x000000),
        LV_PART_MAIN
    );
    
    while(1)
    {
        printf("UI while start!\n");
        lv_timer_handler();
        printf("lv_timer_handler start!\n");
        vTaskDelay(pdMS_TO_TICKS(2));
    }
}

void ui_flush_complete_from_isr(void)
{
    lv_display_t *display = flushing_display;

    if(display != NULL)
    {
        flushing_display = NULL;
        lv_display_flush_ready(display);
    }
}