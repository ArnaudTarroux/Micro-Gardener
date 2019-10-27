#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "config/config.h"
#include "event_source.h"
#include "mg_errors.h"
#include "event_loop/event_loop.h"
#include "wifi/wifi.h"
#include "timer/timer.h"
#include "mqtt/mqtt.h"
#include "leds/leds.h"
#include "action/action_dispatcher.h"

#define MAIN_LOG_TAG "MAIN"

void on_wifi_connected(void* handler_args, esp_event_base_t base, int32_t event, void* event_data);

void app_main()
{
    mg_config_err conf_err;

    init_action_dispatcher();
    init_event_loop();
    conf_err = init_config();
    if (conf_err != MG_CONFIG_OK) {
        ESP_LOGE(MAIN_LOG_TAG, "Error during config. MG_CONFIG_ERR : %d", conf_err);
        vTaskDelay(30000 / portTICK_PERIOD_MS);
        esp_restart();
    }

    event_loop_register(MG_WIFI_CONNECTED, on_wifi_connected);

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    init_wifi();
}

void start_modules() {
    ESP_LOGI(MAIN_LOG_TAG, "Modules gonna be started");

    init_mqtt();
    init_leds();
}

void on_wifi_connected(void* handler_args, esp_event_base_t base, int32_t event, void* event_data) {
    init_timer();
    start_modules();
}