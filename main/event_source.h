#ifndef EVENT_SOURCE_H_
#define EVENT_SOURCE_H_

#include "esp_event.h"

ESP_EVENT_DECLARE_BASE(MG_EVENTS);

typedef enum {
    MG_WIFI_NOT_CONFIGURED = 1,
    MG_WIFI_CONNECTED,
} mg_event;

typedef enum {
    MG_MQTT_LEDS_STATUS = 10001,
    MG_MQTT_UPDATE_DIM = 10002,
} mqtt_event;

#endif