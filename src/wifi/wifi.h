#ifndef WIFI_H_
#define WIFI_H_

#include "../config/config.h"

#define WIFI_SSID "microGardener"
#define WIFI_PASSWORD "microGardener-ppp"
#define WIFI_LOG_TAG "WIFI"
#define WIFI_US_STASK_DEPTH 4096

void wifi_init(main_config *config);

#endif