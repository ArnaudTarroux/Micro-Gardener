#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/gpio.h"
#include <esp_log.h>
#include "../event_loop/event_loop.h"
#include "../event_source.h"
#include "../event_source.h"
#include "../temperature/temperature.h"
#include "fan.h"

#define FAN_GPIO GPIO_NUM_5

float currentTemperature(0);

void fan_task(void *parameters) {
    while(1) {
        if (currentTemperature > 23) {
            gpio_set_level(FAN_GPIO, 1);
        } else {
            gpio_set_level(FAN_GPIO, 0);
        }

        ESP_LOGI(FAN_LOG_TAG, "FAN TASK %0.2f", currentTemperature);

        vTaskDelay(10000);
    }
}

void on_temperature_changed(void* handler_args, esp_event_base_t base, int32_t event, void* event_data) {
    ESP_LOGI(FAN_LOG_TAG, "Temperature changed");
    currentTemperature = get_temperature();
}

void init_fan(main_config *config) {
    ESP_LOGI(FAN_LOG_TAG, "Initialized");
    gpio_pad_select_gpio(FAN_GPIO);
    gpio_set_direction(FAN_GPIO, GPIO_MODE_OUTPUT);
    event_loop_register(MG_EVENT_TEMPERATURE_CHANGED, on_temperature_changed);
    xTaskCreate(fan_task, "fan_task", TEMPERATURE_US_STASK_DEPTH, NULL, 2, NULL);
}