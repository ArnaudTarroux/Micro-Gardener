#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include "mqtt_client.h"
#include "esp_wifi.h"
#include "ArduinoJson.h"

#include "mqtt.h"

esp_mqtt_client_handle_t mqtt_client;

void mqtt_task(void *parameters) {

    while (1) {
        int msg_id = esp_mqtt_client_publish(mqtt_client, "/talala", "data_3", 0, 1, 0);
        ESP_LOGI(MQTT_LOG_TAG, "Message published %d", msg_id);

        vTaskDelay(30000);
    }
}

esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event) {
    ESP_LOGI(MQTT_LOG_TAG, "Event dispatched from event loop event_id=%d", event->event_id);

    if (event->event_id == MQTT_EVENT_CONNECTED) {
        // xTaskCreate(mqtt_task, "mqtt_task", MQTT_STASK_DEPTH, NULL, 2, NULL);
        esp_mqtt_client_subscribe(mqtt_client, "/mg/control", 0);
    }

    if (event->event_id == MQTT_EVENT_DATA) {
        ESP_LOGI(MQTT_LOG_TAG, " ");
        ESP_LOGI(MQTT_LOG_TAG, " ");
        ESP_LOGI(MQTT_LOG_TAG, "MQTT_EVENT_DATA");
        ESP_LOGI(MQTT_LOG_TAG, "TOPIC=%.*s\r\n", event->topic_len, event->topic);
        ESP_LOGI(MQTT_LOG_TAG, "DATA=%.*s\r\n", event->data_len, event->data);

        DynamicJsonDocument doc(1024);
        deserializeJson(doc, event->data);

        const char *control = doc["control"];
        const char *action = doc["action"];

        ESP_LOGI(MQTT_LOG_TAG, "CONTROL=%s", control);
        ESP_LOGI(MQTT_LOG_TAG, "ACTION=%s", action);
    }

    return ESP_OK;
}

void init_mqtt(main_config *config) {
    ESP_LOGI(MQTT_LOG_TAG, "initialized");

    esp_mqtt_client_config_t mqtt_cfg = {
        .event_handle = mqtt_event_handler,
        .host = "192.168.1.23",
        .uri = "mqtt://guest:guest@192.168.1.23:1883",
        .port = 1883,
        .client_id = "micro-gardener",
        .username = "guest",
        .password = "guest",
    };
    
    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    if (NULL == mqtt_client) {
        ESP_LOGI(MQTT_LOG_TAG, "CANNOT CREATE MQTT CLIENT");
        return;
    }

    esp_mqtt_client_start(mqtt_client);
}