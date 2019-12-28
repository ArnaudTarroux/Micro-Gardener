#ifndef CORE_H_
#define CORE_H_

#include "config/config.h"
#include "config/plugins/wifi_config.cpp"
#include "config/plugins/sntp_config.cpp"
#include "wifi/wifi.h"

typedef enum {
    CORE_OK = 1,
    CORE_WIFI_AP,
} core_err_t;

core_err_t start_core(Config *config);

#endif