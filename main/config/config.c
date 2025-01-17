#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"

#include "config.h"
#include "event_source.h"
#include "../event_loop/event_loop.h"

#include "wifi_config.h"
#include "mqtt_config.h"
#include "leds_config.h"
#include "timer_config.h"

nvs_handle config_handle;

mg_config_err init_config() {
    ESP_LOGI(CONFIG_LOG_TAG, "initialized");
    
    mg_config_err config_err;
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK( nvs_flash_erase() );
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    ESP_ERROR_CHECK( nvs_open(CONFIG_STORAGE_NAMESPACE, NVS_READWRITE, &config_handle) );

    /**
     * WIFI CONFIG 
     */
    config_err = init_wifi_config(config_handle);
    if (config_err != MG_CONFIG_OK) return config_err;

    /**
     * MQTT CONFIG 
     */
    init_mqtt_config(config_handle);

    /**
     * LEDS CONFIG 
     */
    init_leds_config(config_handle);

    /**
     * TIMER CONFIG 
     */
    init_timer_config(config_handle);

    return MG_CONFIG_OK;
}

void get_wifi_ssid(char *ssid) {
    size_t ssid_size = MAX_KEY_CONFIG_SIZE - 1;
    nvs_get_str(config_handle, "ssid", ssid, &ssid_size);
}

void get_wifi_password(char *password) {
    size_t password_size = MAX_KEY_CONFIG_SIZE - 1;
    nvs_get_str(config_handle, "w_password", password, &password_size);
}

void get_mqtt_uri(char *uri) {
    size_t uri_size = MAX_KEY_CONFIG_SIZE - 1;
    nvs_get_str(config_handle, "mqtt_uri", uri, &uri_size);
}

void get_leds_dim(int *dim) {
    nvs_get_i16(config_handle, "leds_dim", dim);
}

void set_leds_dim(int dim) {
    nvs_set_i16(config_handle, "leds_dim", dim);
}

mg_config_err get_timer_period(int *start, int *stop) {
    esp_err_t err_timer_start = nvs_get_i16(config_handle, "timer_start", start);
    esp_err_t err_timer_stop = nvs_get_i16(config_handle, "timer_stop", stop);
    if (err_timer_start == ESP_ERR_NVS_NOT_FOUND || err_timer_stop == ESP_ERR_NVS_NOT_FOUND) {
        return MG_CONFIG_TIMER_NOT_CONFIG;
    }

    return MG_CONFIG_OK;
}

void set_timer_period(int start, int stop) {
    nvs_set_i16(config_handle, "timer_start", start);
    nvs_set_i16(config_handle, "timer_stop", stop);
}