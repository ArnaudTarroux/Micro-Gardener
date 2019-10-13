#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <esp_log.h>
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "AsyncJson.h"
#include "ArduinoJson.h"

#include "./httpd.h"
#include "../event_source.h"
#include "../event_loop/event_loop.h"
#include "../config/config.h"

AsyncWebServer server(80);

void httpd_init(main_config *config) {
  server.on("/health", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(200, "application/json", "{\"health\": \"OK\"}");
  });

  server.on("/api/temp", HTTP_POST, [](AsyncWebServerRequest *request) {
      if (!request->hasParam("max_temp", true) && !request->hasParam("max_temp", true)) {
        request->send(400, "application/json", "{\"message\": \"min_temp and max_temp are required\"}");
        return;
      }

      temp_config temp_config_ = {
        .min_temp = atof(request->getParam("min_temp", true)->value().c_str()),
        .max_temp = atof(request->getParam("max_temp", true)->value().c_str()),
      };

      event_loop_dispatch(MG_EVENT_CONF_TEMP_MUST_UPDATE, &temp_config_);
      
      request->send(200, "application/json", "{\"message\": \"temp_updated\"}");
  });

  server.on("/api/leds", HTTP_POST, [](AsyncWebServerRequest *request) {
      if (
        !request->hasParam("start_hours", true) && !request->hasParam("start_minutes", true) &&
        !request->hasParam("end_hours", true) && !request->hasParam("end_minutes", true)
      ) {
        request->send(400, "application/json", "{\"message\": \"start_hours, start_minutes, end_hours, end_minutes are required\"}");
        return;
      }

      daytime_config daytime_config_ = {
        .start = {
          .hours = atoi(request->getParam("start_hours", true)->value().c_str()),
          .minutes = atoi(request->getParam("start_minutes", true)->value().c_str()),
        },
        .end = {
          .hours = atoi(request->getParam("end_hours", true)->value().c_str()),
          .minutes = atoi(request->getParam("end_minutes", true)->value().c_str()),
        },
      };

      set_daytime_config(daytime_config_);
      
      request->send(200, "application/json", "{\"message\": \"leds-updated\"}");
  });

  server.begin();
}