#ifndef ACTION_DISPATCHER_H_
#define ACTION_DISPATCHER_H_

#include "esp_event_base.h"
#include "event_source.h"
#include "action_parser.h"

#define ACTION_D_LOG_TAG "ACTION_DISPATCHER"

ESP_EVENT_DECLARE_BASE(MG_ACTIONS);

typedef enum {
    MG_ACTION_LEDS_ON_OFF =         10001,
    MG_ACTION_UPDATE_DIM =          10002,
    MG_ACTION_UPDATE_TIMER_PERIOD = 10003,
    MG_ACTION_UPDATE_FAN_DUTY =     10004
} actions;

void action_dispatch(char *data);
void action_register(actions action, esp_event_handler_t handler);
void init_action_dispatcher();

#endif