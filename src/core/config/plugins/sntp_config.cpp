#include "../config.h"

#define SNTP_KV_OFFSET "sntp_offset"
#define SNTP_DEFAULT_OFFSET 3600

using namespace config;

namespace config {
    class SntpConfig: public ConfigPlugin {
    private:
        nvs_handle *handle;

    public:
        config_err_t start(nvs_handle *handle) {
            Serial.println("SNTPConfig Starting");
            this->handle = handle;

            esp_err_t err;
            int16_t offsetTime;
            err = nvs_get_i16(*this->handle, SNTP_KV_OFFSET, &offsetTime);
            if (err == ESP_ERR_NVS_NOT_FOUND) {
                nvs_set_i16(*this->handle, SNTP_KV_OFFSET, SNTP_DEFAULT_OFFSET);
            }

            return CONFIG_OK;
        }
    };
}