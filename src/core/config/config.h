#ifndef CONFIG_H_
#define CONFIG_H_

#include <Arduino.h>
#include <array>
#include "nvs_flash.h"
#include "nvs.h"

#define CONFIG_STORAGE_NAMESPACE "mg_conf"
#define MAX_KEY_CONFIG_SIZE 517

using namespace std;

typedef enum {
    CONFIG_OK = 1,
    CONFIG_NOT_SETUP,
    CONFIG_WIFI_NOT_SETUP,
} config_err_t;

namespace config {
    class ConfigPlugin {
    public:
        virtual config_err_t start(nvs_handle *handle) = 0;
    };

    class Config 
    {
    public:
        config_err_t begin();
        void addPlugin(ConfigPlugin *plugin);

        // setters
        void save_i8(const char *key, int8_t value);
        void save_i16(const char *key, int16_t value);
        void save_str(const char *key, const char *value);
        // getters
        void get_i8(const char *key, int8_t *outValue);
        void get_i16(const char *key, int16_t *outValue);
        void get_str(const char *key, char *outValue);

    private:
        int configPluginCount = 0;
        nvs_handle configHandle;
        ConfigPlugin *configPlugins[] = {};
    };
}

#endif