#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_

#include "../config/config.h"

#define TEMPERATURE_LOG_TAG "TEMPERATURE"
#define TEMPERATURE_GPIO_NUM 4
#define TEMPERATURE_US_STASK_DEPTH 4096
#define TEMPERATURE_DELAY_BETWEEN 30000

void temperature_init(main_config *config);
float get_temperature();
float get_humidity();

#endif