#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "dht22.h"
#include "weather.h"
#include "mqtt/mqtt.h"

void weatherTask(void *parameters);
void send_weather_stats();

void init_weather() {
    ESP_LOGI(WEATHER_LOG_TAG, "initialized");
    setDHTgpio(SENSOR_GPIO);

    xTaskCreate(weatherTask, "weatherTask", WEATHER_STASK_DEPTH, NULL, 3, NULL);
}

int err_read;
void weatherTask(void *parameters) {
    while(1) {
        err_read = readDHT();
        if (err_read != DHT_OK) {

            ESP_LOGI(WEATHER_LOG_TAG, "cannot read weather :: %d", err_read);

            // sprintf(message, "{\"temperature\": %2.2f, \"humidity\": %2.2f}", 0.0, 0.0);
            // vTaskDelay(30000 / portTICK_PERIOD_MS);
            // continue;
        }

        send_weather_stats();
        vTaskDelay(120000 / portTICK_PERIOD_MS);
    }
}

float readTemperature() {
    return getTemperature();
}
float readHumidity() {
    return getHumidity();
}

void send_weather_stats() {
    char message[41];
    sprintf(message, "{\"temperature\": %2.2f, \"humidity\": %2.2f}", readTemperature(), readHumidity());
    ESP_LOGI(WEATHER_LOG_TAG, "Sending :: %s", message);
    publish_message("mg/stats/weather", message);
}