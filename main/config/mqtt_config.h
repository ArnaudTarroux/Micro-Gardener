#ifndef MQTT_CONFIG_H_
#define MQTT_CONFIG_H_

#include "nvs.h"
#include "config.h"
#include "mg_errors.h"

#define MQTT_CONFIG_LOG_TAG "MQTT_CONFIG"
#define MQTT_CONFIG_DEFAULT_URI "mqtt://192.168.1.24:1883"

mg_config_err init_mqtt_config(nvs_handle config_handle);

#endif