#ifndef EVENT_SOURCE_H_
#define EVENT_SOURCE_H_

#include "esp_event.h"

ESP_EVENT_DECLARE_BASE(MG_EVENTS);

typedef enum {
    MG_WIFI_NOT_CONFIGURED = 1,
    MG_WIFI_CONNECTED,
    MG_STATE_DAY,
    MG_STATE_NIGHT,
} mg_event;

#endif