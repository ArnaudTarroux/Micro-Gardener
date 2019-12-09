#include <Arduino.h>
#include "nvs.h"
#include "nvs_flash.h"
#include "../config.h"

#define WIFI_CONFIG_LOG_TAG "WIFI_CONFIG"
#define WIFI_KV_SSID "w_ssid"
#define WIFI_KV_PASSWORD "w_password"

using namespace config;

namespace config {
    class WifiConfig: public ConfigPlugin {
    private:
        nvs_handle *handle;
        size_t kvSize = MAX_KEY_CONFIG_SIZE - 1;

    public:
        config_err_t start(nvs_handle *handle) {
            Serial.println("WifiConfig Starting");
            this->handle = handle;

            esp_err_t errSsid, errPassword;
            char ssid[MAX_KEY_CONFIG_SIZE];
            
            errSsid = nvs_get_str(*this->handle, WIFI_KV_SSID, ssid, &this->kvSize);
            
            char password[MAX_KEY_CONFIG_SIZE];
            size_t passwordSize = MAX_KEY_CONFIG_SIZE - 1;
            errPassword = nvs_get_str(*this->handle, WIFI_KV_PASSWORD, password, &passwordSize);

            if (errSsid == ESP_ERR_NVS_NOT_FOUND || errPassword == ESP_ERR_NVS_NOT_FOUND) {
                return CONFIG_WIFI_NOT_SETUP;
            }

            return CONFIG_OK;
        }

        void getCredentials(char *ssid, char *password)
        {
            nvs_get_str(*this->handle, WIFI_KV_SSID, ssid, &this->kvSize);
            nvs_get_str(*this->handle, WIFI_KV_PASSWORD, password, &this->kvSize);
        }
    };
}