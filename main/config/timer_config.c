#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"

#include "timer_config.h"

mg_config_err init_timer_config(nvs_handle config_handle) {
    ESP_LOGI(TIMER_CONFIG_LOG_TAG, "initialized");

    return MG_CONFIG_OK;
}