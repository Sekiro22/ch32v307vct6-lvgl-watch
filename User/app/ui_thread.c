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

    TFT_setWindow(area->x1, area->y1, area->x2, area->y2);
    TFT_writePixels((const uint16_t *)pixel_map, width * height);

    lv_display_flush_ready(display);
}

void ui_thread(void * ui_arg)
{
    lv_display_t *display;
    printf("UI_Thread start!\n");

    lv_init();
    lv_tick_set_cb(lvgl_tick_get);

    display = lv_display_create(TFT_WIDTH, TFT_HEIGHT);
    lv_display_set_color_format(display, LV_COLOR_FORMAT_RGB565);

    lv_display_set_buffers(display, lvgl_draw_buffer, NULL, sizeof(lvgl_draw_buffer), LV_DISPLAY_RENDER_MODE_PARTIAL);

    lv_display_set_flush_cb(display, lvgl_flush);

    ch32v307_gc9a01_ui_init("");
    lv_screen_load(screen_main_create());

    lv_obj_set_style_bg_color(
        lv_screen_active(),
        lv_color_hex(0x000000),
        LV_PART_MAIN
    );

    
    while(1)
    {
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}