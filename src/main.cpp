#include <stdio.h>
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include "config/config.h"
#include "wifi/wifi.h"
#include "httpd/httpd.h"
#include "temperature/temperature.h"
#include "fan/fan.h"
#include "led/led.h"
#include "timer/timer.h"
#include "state/state.h"
#include "mqtt/mqtt.h"
#include "event_source.h"
#include "event_loop/event_loop.h"

#define EXT_WIFI_SSID "SFR_9878"
#define EXT_WIFI_PASSWORD "ertyerty"

wifi_config wifi_config_ {
  .ap_ssid = WIFI_SSID,
  .ap_password = WIFI_PASSWORD,
  .sta_ssid = EXT_WIFI_SSID,
  .sta_password = EXT_WIFI_PASSWORD,
  .is_sta_ready = 1, 
};

daytime_config daytime_config_ {
  .start = { .hours = 19, .minutes = 0 },
  .end = { .hours = 6, .minutes = 0 },
};

main_config main_config_ { .wifi = wifi_config_, .temp = {}, .daytime = daytime_config_ };

void setup() {
  Serial.begin(9600);
  esp_log_level_set("*", ESP_LOG_INFO);

  init_event_loop();
  config_init(&main_config_);
  wifi_init(&main_config_);
  temperature_init(&main_config_);
  httpd_init(&main_config_);
  init_fan(&main_config_);
  init_led(&main_config_);
  init_timer(&main_config_);
  init_state(&main_config_);
  init_mqtt(&main_config_);
}

void loop() {
  vTaskDelay(10000 / portTICK_PERIOD_MS);
}