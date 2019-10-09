#ifndef FAN_H_
#define FAN_H_

#include "../config/config.h"

#define FAN_LOG_TAG "FAN"
#define TEMPERATURE_US_STASK_DEPTH 4096

void init_fan(main_config *config);

#endif