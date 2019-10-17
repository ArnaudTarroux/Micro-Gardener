#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "wifi.h"

void say_hello_task(void *parameters) {
    while (1) {
        ESP_LOGI(WIFI_LOG_TAG, "HELLO FROM WIFI");
        ESP_LOGI(WIFI_LOG_TAG, " WIFI");

        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void init_wifi() {
    xTaskCreate(say_hello_task, "say_hello_task", WIFI_US_STASK_DEPTH, NULL, 2, NULL);
}