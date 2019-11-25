#ifndef WEATHER_H_
#define WEATHER_H_

#include "driver/gpio.h"

#define WEATHER_LOG_TAG "WEATHER"
#define WEATHER_STASK_DEPTH 4096
#define SENSOR_GPIO GPIO_NUM_4

void init_weather();
float readTemperature();
float readHumidity();

#endif