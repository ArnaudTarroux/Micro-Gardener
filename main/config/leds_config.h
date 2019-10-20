#ifndef LEDS_CONFIG_H_
#define LEDS_CONFIG_H_

#include "nvs.h"
#include "config.h"
#include "mg_errors.h"

#define LEDS_CONFIG_LOG_TAG "LEDS_CONFIG"
#define LEDS_DIM_DEFAULT 80

mg_config_err init_leds_config(nvs_handle config_handle);

#endif