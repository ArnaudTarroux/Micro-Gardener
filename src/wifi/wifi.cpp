#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <esp_wifi.h>
#include <esp_log.h>
#include "esp_event_loop.h"

#include "./wifi.h"

static EventGroupHandle_t s_wifi_event_group;
const int WIFI_CONNECTED_BIT = BIT0;

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
const char * ext_ssid;
const char * ext_password;
static int wifi_sta_connection_max_retry = 10;
static int wifi_sta_connection_retry_count = 0;

void wifi_start_ap();
void wifi_start_sta();
static esp_err_t event_handler(void *ctx, system_event_t *event);

void wifi_init(main_config *config) {
    ESP_LOGI(WIFI_LOG_TAG, "init");

    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    s_wifi_event_group = xEventGroupCreate();
    tcpip_adapter_init();
    wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_config);

    if (!config->wifi.is_sta_ready) {
        wifi_start_ap();
        return;
    }

    ext_ssid = config->wifi.sta_ssid;
    ext_password = config->wifi.sta_password;

    wifi_start_sta();
}

void wifi_start_ap() {
    esp_wifi_stop();
    ESP_LOGI(WIFI_LOG_TAG, "AP mode started SSID=%s PASSWORD=%s", ssid, password);

    wifi_config_t wifi_config = {};
    wifi_config.ap.authmode = WIFI_AUTH_WPA2_PSK;
    wifi_config.ap.max_connection = 1;
    memcpy(wifi_config.ap.ssid, ssid, sizeof(wifi_config.ap.ssid));
    memcpy(wifi_config.ap.password, password, sizeof(wifi_config.ap.password));

    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_AP) );
    ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
}

void wifi_start_sta() {
    if (ext_password == NULL || ext_ssid == NULL) {
        ESP_LOGI(WIFI_LOG_TAG, "EXTERNAL ssid or password is missing");
        return;
    }
    esp_wifi_stop();

    xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);

    ESP_LOGI(WIFI_LOG_TAG, "CONNECTING TO EXTERNAL WIFI %s", ext_ssid);

    wifi_config_t wifi_config = {};
    memcpy(wifi_config.sta.password, ext_password, sizeof(wifi_config.sta.password));
    memcpy(wifi_config.sta.ssid, ext_ssid, sizeof(wifi_config.sta.ssid));
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
}

static esp_err_t event_handler(void *ctx, system_event_t *event) {
    ESP_LOGI(WIFI_LOG_TAG, "EVENT_ID=%d", event->event_id);

    switch(event->event_id) {
        case SYSTEM_EVENT_AP_STACONNECTED:
            ESP_LOGI(WIFI_LOG_TAG, "station:"MACSTR" join, AID=%d",
                    MAC2STR(event->event_info.sta_connected.mac),
                    event->event_info.sta_connected.aid);
            break;
        case SYSTEM_EVENT_AP_STADISCONNECTED:
            ESP_LOGI(WIFI_LOG_TAG, "station:"MACSTR"leave, AID=%d",
                    MAC2STR(event->event_info.sta_disconnected.mac),
                    event->event_info.sta_disconnected.aid);
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            ESP_LOGI(WIFI_LOG_TAG, "STA: %s", ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
            break;
        case SYSTEM_EVENT_STA_CONNECTED:
            ESP_LOGI(WIFI_LOG_TAG, "STA: CONNECTED");
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            ESP_LOGI(WIFI_LOG_TAG, "STA: DISCONNECTED");
            {
                if (wifi_sta_connection_retry_count < wifi_sta_connection_max_retry) {
                    esp_wifi_connect();
                    wifi_sta_connection_retry_count++;
                    ESP_LOGI(WIFI_LOG_TAG, "STA: retry....");
                }
            }
            break;
        case SYSTEM_EVENT_STA_START:
            ESP_LOGI(WIFI_LOG_TAG, "STA: STARTED");
            esp_wifi_connect();
            break;
        default:
            break;
    }
    return ESP_OK;
}
