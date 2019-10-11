#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <WiFi.h>

#include "./wifi.h"

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
const char * ext_ssid;
const char * ext_password;

void wifi_start_ap();
void wifi_start_sta();

void wifi_init(main_config *config) {
    ESP_LOGI(WIFI_LOG_TAG, "init");

    if (!config->wifi.is_sta_ready) {
        wifi_start_ap();
        return;
    }

    ext_ssid = config->wifi.sta_ssid;
    ext_password = config->wifi.sta_password;

    wifi_start_sta();
}

void wifi_start_ap() {
    ESP_LOGI(WIFI_LOG_TAG, "SETUP WIFI IN AP MODE");
    WiFi.mode(WIFI_MODE_AP);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    } 

    ESP_LOGI(WIFI_LOG_TAG, "WIFI connected in AP mode");
}

void wifi_start_sta() {
    if (ext_password == NULL || ext_ssid == NULL) {
        ESP_LOGI(WIFI_LOG_TAG, "EXTERNAL ssid or password is missing");
        return;
    }

    ESP_LOGI(WIFI_LOG_TAG, "SETUP WIFI IN STA MODE");
    WiFi.mode(WIFI_MODE_STA);
    WiFi.begin(ext_ssid, ext_password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    } 

    Serial.printf("WIFI connected in STA mode. IP:");
    Serial.println(WiFi.localIP());
}
