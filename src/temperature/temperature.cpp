#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <math.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#include "temperature.h"
#include "../event_loop/event_loop.h"
#include "../event_source.h"

#define DHTPIN TEMPERATURE_GPIO_NUM
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
float actualTemperature(0);
float actualHumidity(0);

void temperature_task(void *parameters);

void temperature_init(main_config *config) {
    ESP_LOGI(TEMPERATURE_LOG_TAG, "initialized");
    dht.begin();
    
    xTaskCreate(temperature_task, "temperature_task", TEMPERATURE_US_STASK_DEPTH, NULL, 2, NULL);
}

void temperature_task(void *parameters) {
    while(1) {
        float humidity = dht.readHumidity();
        float temperature = dht.readTemperature();
        if (isnan(humidity) || isnan(temperature)) {
            ESP_LOGI(TEMPERATURE_LOG_TAG, "Failed to read temperature or humidity");
            vTaskDelay(TEMPERATURE_DELAY_BETWEEN / portTICK_PERIOD_MS);
            continue;
        }

        if (actualHumidity != humidity) {
            actualHumidity = humidity;
            event_loop_dispatch(MG_EVENT_HUMIDITY_CHANGED, &humidity);
        }

        if (actualTemperature != temperature) {
            actualTemperature = temperature;
            event_loop_dispatch(MG_EVENT_TEMPERATURE_CHANGED, &temperature);
        }

        vTaskDelay(TEMPERATURE_DELAY_BETWEEN / portTICK_PERIOD_MS);
    }
}

float get_temperature() {
    return actualTemperature;
}

float get_humidity() {
    return actualHumidity;
}
