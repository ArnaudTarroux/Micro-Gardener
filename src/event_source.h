#ifndef EVENT_SOURCE_H_
#define EVENT_SOURCE_H_

#include "esp_event.h"

ESP_EVENT_DECLARE_BASE(MG_EVENTS);

enum mg_event {
    MG_EVENT_TEMPERATURE_CHANGED,
    MG_EVENT_HUMIDITY_CHANGED,
};

#endif