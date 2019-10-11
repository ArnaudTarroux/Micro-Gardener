#include <esp_log.h>
#include "string.h"
#include "config.h"
#include "../event_source.h"
#include "../event_loop/event_loop.h"

void set_config(void *key, const void *value) {
    memccpy(key, value, 0, sizeof(key));
}

void on_temp_conf_update(void* handler_args, esp_event_base_t base, int32_t event, void* event_data);

void config_init(main_config *config) {
    event_loop_register(MG_EVENT_CONF_TEMP_MUST_UPDATE, on_temp_conf_update);
}

void on_temp_conf_update(void* handler_args, esp_event_base_t base, int32_t event, void* event_data) {
    ESP_LOGI(CONFIG_LOG_TAG, "temperature conf must update");
    temp_config *temp_config_ = (struct temp_config*)event_data;
    
    ESP_LOGI(CONFIG_LOG_TAG, "CONF MIN TEMP %f", temp_config_->min_temp);
    ESP_LOGI(CONFIG_LOG_TAG, "CONF MAX TEMP %f", temp_config_->max_temp);
}