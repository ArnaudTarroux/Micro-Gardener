#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include <esp_event_base.h>
#include <event_source.h>

#define EVENT_LOOP_LOG_TAG "EVENT_LOOP"

void event_loop_dispatch(mg_event event, void* event_data);
void event_loop_register(mg_event event, esp_event_handler_t handler);
void init_event_loop();

#endif