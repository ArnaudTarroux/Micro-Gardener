#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"

#include "wifi_config.h"
#include "event_source.h"
#include "mg_errors.h"
#include "../event_loop/event_loop.h"

mg_config_err init_wifi_config(nvs_handle config_handle) {
    ESP_LOGI(WIFI_CONFIG_LOG_TAG, "initialized");

    esp_err_t err_ssid, err_password;
    char ssid[MAX_KEY_CONFIG_SIZE];
    size_t ssid_size = MAX_KEY_CONFIG_SIZE - 1;
    err_ssid = nvs_get_str(config_handle, "ssid", ssid, &ssid_size);
    
    char password[MAX_KEY_CONFIG_SIZE];
    size_t password_size = MAX_KEY_CONFIG_SIZE - 1;
    err_password = nvs_get_str(config_handle, "w_password", password, &password_size);

    if (err_ssid == ESP_ERR_NVS_NOT_FOUND || err_password == ESP_ERR_NVS_NOT_FOUND) {
        return MG_CONFIG_WIFI_NOT_CONFIG;
    }

    return MG_CONFIG_OK;
}
