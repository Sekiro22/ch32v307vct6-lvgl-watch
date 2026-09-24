/**
 * @file ch32v307_gc9a01_ui_gen.h
 */

#ifndef LVGL_PRO_CH32V307_GC9A01_UI_GEN_H
#define LVGL_PRO_CH32V307_GC9A01_UI_GEN_H

#ifndef UI_SUBJECT_STRING_LENGTH
#define UI_SUBJECT_STRING_LENGTH 256
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
    #include "lvgl.h"
    #include "lvgl_private.h"
#else
    #include "lvgl/lvgl.h"
    #include "lvgl/lvgl_private.h"
#endif

#if defined(LV_USE_XML) && LV_USE_XML
    #include "lv_xml/lv_xml.h"
#endif



/* Prototypes for target functions, needed by responsive const definitions */

void ch32v307_gc9a01_ui_set_target(uint32_t target);
uint32_t ch32v307_gc9a01_ui_get_target(void);
bool ch32v307_gc9a01_ui_check_target(uint32_t target);

/*********************
 *      DEFINES
 *********************/

#define CH32V307_GC9A01_UI_TARGET_UNDEFINED  (0 << 1)
#define CH32V307_GC9A01_UI_TARGET_GC9A01     (1 << 1)
#define CH32V307_GC9A01_UI_TARGET_ALL        0x0FFFFFFF

/* By default compile for all targets, allowing to switch to any targets at runtime */
#ifndef CH32V307_GC9A01_UI_COMPILE_TARGET
#define CH32V307_GC9A01_UI_COMPILE_TARGET CH32V307_GC9A01_UI_TARGET_ALL
#endif

#define CH32V307_GC9A01_UI_CHECK_COMPILE_TARGET(target) (CH32V307_GC9A01_UI_COMPILE_TARGET & (target) ? 1 : 0)

#ifndef LV_XML_EVAL_STRING_BUF_SIZE
    #define LV_XML_EVAL_STRING_BUF_SIZE 256
#endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL VARIABLES
 **********************/

/*-------------------
 * Permanent screens
 *------------------*/

/*----------------
 * Global styles
 *----------------*/

/*----------------
 * Fonts
 *----------------*/

/* Targets: any */
extern lv_font_t * font_data_10;
extern lv_font_t * font_data_12;


/*----------------
 * Images
 *----------------*/



/*----------------
 * Subjects
 *----------------*/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/*----------------
 * Event Callbacks
 *----------------*/

/**
 * Initialize the component library
 */

void ch32v307_gc9a01_ui_init_gen(const char * asset_path);

/**********************
 *      MACROS
 **********************/

/**********************
 *   POST INCLUDES
 **********************/

/*Include all the widgets, components and screens of this library*/
#include "screens/screen_main_gen.h"

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LVGL_PRO_CH32V307_GC9A01_UI_GEN_H*/