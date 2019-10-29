#ifndef TIMER_CONFIG_H_
#define TIMER_CONFIG_H_

#include "nvs.h"
#include "config.h"
#include "mg_errors.h"

#define TIMER_CONFIG_LOG_TAG "TIMER_CONFIG"

mg_config_err init_timer_config(nvs_handle config_handle);

#endif