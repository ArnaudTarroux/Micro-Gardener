#include <array>
#include <Arduino.h>
#include "core/core.h"
#include "sntp/sntp.h"
#include "climate/climate.h"
#include "daystate/daystate.h"
#include "led/led.h"

using namespace config;
using namespace microgardener;

Sntp sntp = Sntp(0);
Climate climate;

void setup() {
  Serial.begin(9600);
  Config config;
  
  core_err_t core_err = start_core(&config);
  if (core_err != CORE_OK) {
    return;
  }

  int16_t offsetTime;
  config.get_i16(SNTP_KV_OFFSET, &offsetTime);
  sntp = Sntp(offsetTime);
  sntp.begin();
  
  Led led;
  led.start();

  DayState daystate;
  daystate.start(sntp);
}

void loop() {
  Serial.printf("t: %2.2f - h: %3.2f \n", climate.getTemperature(), climate.getHumidity());
  delay(10000);
}