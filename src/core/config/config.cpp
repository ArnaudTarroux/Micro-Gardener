#include <Arduino.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"

#include "config.h"

#define CONFIG_LOG_TAG "CONFIG"

namespace config
{
    config_err_t Config::begin()
    {
        esp_err_t err = nvs_flash_init();
        if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
            ESP_ERROR_CHECK( nvs_flash_erase() );
            err = nvs_flash_init();
        }

        nvs_open(CONFIG_STORAGE_NAMESPACE, NVS_READWRITE, &configHandle);

        Serial.printf("plugins :: %d \n", configPluginCount);

        config_err_t configErr;
        for (int i = 0; i < configPluginCount; i++) {
            ConfigPlugin *cf = configPlugins[i];
            configErr = cf->start(&configHandle);
            if (configErr == CONFIG_OK) {
                continue;
            }

            return configErr;
        }

        return CONFIG_OK;
    }

    void Config::addPlugin(ConfigPlugin *plugin)
    {
        configPlugins[configPluginCount] = plugin;
        configPluginCount++;
    }

    void Config::save_i8(const char *key, int8_t value)
    {
        nvs_set_i8(configHandle, key, value);
    }

    void Config::save_i16(const char *key, int16_t value)
    {
        nvs_set_i16(configHandle, key, value);
    }

    void Config::save_str(const char *key, const char *value)
    {
        nvs_set_str(configHandle, key, value);
    }

    void Config::get_i8(const char *key, int8_t *outValue)
    {
        nvs_get_i8(configHandle, key, outValue);
    }

    void Config::get_i16(const char *key, int16_t *outValue)
    {
        nvs_get_i16(configHandle, key, outValue);
    }

    void Config::get_str(const char *key, char *outValue)
    {
        size_t lenght = MAX_KEY_CONFIG_SIZE - 1;
        esp_err_t err = nvs_get_str(configHandle, key, outValue, &lenght);
        Serial.printf("get returned ::: %d \n", err);
    }
}