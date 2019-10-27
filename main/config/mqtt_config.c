#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"

#include "mqtt_config.h"
#include "event_source.h"
#include "mg_errors.h"
#include "../event_loop/event_loop.h"

mg_config_err init_mqtt_config(nvs_handle config_handle) {
    ESP_LOGI(MQTT_CONFIG_LOG_TAG, "initialized");
    esp_err_t err_uri;

    char uri[MAX_KEY_CONFIG_SIZE];
    size_t uri_size = MAX_KEY_CONFIG_SIZE - 1;

    err_uri = nvs_get_str(config_handle, "mqtt_uri", uri, &uri_size);
    if (err_uri == ESP_ERR_NVS_NOT_FOUND) {
        nvs_set_str(config_handle, "mqtt_uri", MQTT_CONFIG_DEFAULT_URI);
    }

    return MG_CONFIG_OK;
}
