#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "cJSON.h"

#include "leds_config.h"
#include "event_source.h"
#include "mg_errors.h"

nvs_handle config_handle;

void on_dim_updated(void* handler_args, esp_event_base_t base, int32_t event, void* event_data);

mg_config_err init_leds_config(nvs_handle config_handle) {
    ESP_LOGI(LEDS_CONFIG_LOG_TAG, "initialized");
    config_handle = config_handle;
    esp_err_t err_dim;

    int dim[MAX_KEY_CONFIG_SIZE];
    err_dim = nvs_get_i16(config_handle, "leds_dim", dim);
    if (err_dim == ESP_ERR_NVS_NOT_FOUND) {
        nvs_set_i16(config_handle, "leds_dim", LEDS_DIM_DEFAULT);
    }

    return MG_CONFIG_OK;
}