#ifndef  __APP_KEY_EVENT__
#define __APP_KEY_EVENT__

extern TaskHandle_t key_taskhandle;

void key_event_task(void * key_event_arg);

#endif