#ifndef WIFI_CONFIG_H_
#define WIFI_CONFIG_H_

#include "nvs.h"
#include "config.h"
#include "mg_errors.h"

#define WIFI_CONFIG_LOG_TAG "WIFI_CONFIG"

mg_config_err init_wifi_config(nvs_handle config_handle);

#endif