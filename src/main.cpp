#include <array>
#include <Arduino.h>
#include "core/core.h"

void setup() {
  Serial.begin(9600);
  
  core_err_t core_err = start_core();
  if (core_err != CORE_OK) {
    return;
  }
}

void loop() { }