/**
 * @file ch32v307_gc9a01_ui.c
 */

/*********************
 *      INCLUDES
 *********************/

#include "ch32v307_gc9a01_ui.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void ch32v307_gc9a01_ui_init(const char * asset_path)
{
    LV_LOG("Initializing custom C code using LVGL v%d.%d.%d", LVGL_VERSION_MAJOR, LVGL_VERSION_MINOR, LVGL_VERSION_PATCH);

    ch32v307_gc9a01_ui_init_gen(asset_path);

    /* Add your own custom code here if needed */
}

/**********************
 *   STATIC FUNCTIONS
 **********************/