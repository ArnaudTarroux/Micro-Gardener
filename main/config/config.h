#ifndef CONFIG_H_
#define CONFIG_H_

#include "mg_errors.h"

#define CONFIG_LOG_TAG "CONFIG"
#define CONFIG_STORAGE_NAMESPACE "config"
#define MAX_KEY_CONFIG_SIZE 517

mg_config_err init_config();

void get_wifi_ssid(char *ssid);
void get_wifi_password(char *password);
void get_mqtt_uri(char *uri);
void get_leds_dim(int *dim);
void set_leds_dim(int dim);
mg_config_err get_timer_period(int *start, int *stop);
void set_timer_period(int start, int stop);

#endif