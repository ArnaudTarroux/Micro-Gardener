#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_event_loop.h"
#include "esp_wifi.h"

#include "../config/config.h"
#include "wifi.h"
#include "event_source.h"
#include "event_loop/event_loop.h"

static EventGroupHandle_t s_wifi_event_group;
static const int WIFI_CONNECTED_BIT = BIT0;

void start_sta();
static esp_err_t event_handler(void *ctx, system_event_t *event);

void init_wifi() {
    ESP_LOGI(WIFI_LOG_TAG, "initialized");

    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );

    s_wifi_event_group = xEventGroupCreate();
    tcpip_adapter_init();

    wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_config);

    start_sta();
}

void start_sta() {
    xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);

    wifi_config_t wifi_config = {};

    char ssid[MAX_KEY_CONFIG_SIZE];
    get_wifi_ssid(&ssid);
    char password[MAX_KEY_CONFIG_SIZE];
    get_wifi_password(&password);

    memcpy(wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid));
    memcpy(wifi_config.sta.password, password, sizeof(wifi_config.sta.password));

    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
}

static esp_err_t event_handler(void *ctx, system_event_t *event) {
    switch (event->event_id) {
        case SYSTEM_EVENT_STA_GOT_IP:
            event_loop_dispatch(MG_WIFI_CONNECTED, NULL);
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            ESP_LOGI(WIFI_LOG_TAG, "Disconnected, mode STA");
            {
                vTaskDelay(10000 / portTICK_PERIOD_MS);
                esp_wifi_connect();
            }
            break;
        case SYSTEM_EVENT_STA_START:
            ESP_LOGI(WIFI_LOG_TAG, "started, mode STA");
            esp_wifi_connect();
            break;
        
        default:
            break;
    }

    return ESP_OK;
}