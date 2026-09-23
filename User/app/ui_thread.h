#ifndef __UI_THREAD_H__
#define __UI_THREAD_H__

extern TaskHandle_t ui_taskhandle;

void ui_thread(void * ui_arg);

#endif