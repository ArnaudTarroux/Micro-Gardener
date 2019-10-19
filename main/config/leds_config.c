#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"

#include "leds_config.h"
#include "event_source.h"
#include "mg_errors.h"
#include "../event_loop/event_loop.h"

static nvs_handle config_handle;

void on_config_leds_status(void* handler_args, esp_event_base_t base, int32_t event, void* event_data) {
    int status = *((int*) event_data);
    ESP_LOGI(LEDS_CONFIG_LOG_TAG, "on_led_status %d", status);
    nvs_set_u8(config_handle, "leds_dim", status);
}

mg_config_err init_leds_config(nvs_handle config_handle) {
    ESP_LOGI(LEDS_CONFIG_LOG_TAG, "initialized");
    config_handle = config_handle;
    esp_err_t err_dim;

    uint8_t dim[MAX_KEY_CONFIG_SIZE];
    
    err_dim = nvs_get_u8(config_handle, "leds_dim", dim);
    if (err_dim == ESP_ERR_NVS_NOT_FOUND) {
        nvs_set_u8(config_handle, "leds_dim", LEDS_DIM_DEFAULT);
    }

    mqtt_event_register(MG_MQTT_LEDS_STATUS, on_config_leds_status);

    return MG_CONFIG_OK;
}
