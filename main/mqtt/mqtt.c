#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "cJson.h"

#include "../config/config.h"

#include "event_source.h"
#include "event_loop/event_loop.h"
#include "action/action_dispatcher.h"
#include "mqtt.h"

esp_mqtt_client_handle_t mqtt_client;
esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event);
void publish_event(esp_mqtt_event_handle_t event);

void init_mqtt() {
    ESP_LOGI(MQTT_LOG_TAG, "initialized");

    char uri[MAX_KEY_CONFIG_SIZE];
    get_mqtt_uri(uri);
    esp_mqtt_client_config_t mqtt_cfg = {
        .event_handle = mqtt_event_handler,
        .uri = uri,
    };
    
    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    if (NULL == mqtt_client) {
        ESP_LOGI(MQTT_LOG_TAG, "CANNOT CREATE MQTT CLIENT");
        return;
    }

    esp_mqtt_client_start(mqtt_client);
}

esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event) {
    switch (event->event_id) {
    case MQTT_EVENT_CONNECTED:
        esp_mqtt_client_subscribe(mqtt_client, "/mg/control", 0);
        break;
    case MQTT_EVENT_DATA:
        {
            ESP_LOGI(MQTT_LOG_TAG, "TOPIC=%.*s\r\n DATA=%.*s\r\n", event->topic_len, event->topic, event->data_len, event->data);
            action_dispatch(event->data);
        }
        break;
    
    default:
        break;
    }

    return ESP_OK;
}

void publish_message(char *topic, char *data) {
    esp_mqtt_client_publish(mqtt_client, topic, data, strlen(data), 0, 0);
}