#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_log.h"

#include <NTPClient.h>
#include <WiFiUdp.h>

#include "timer.h"
#include "../event_source.h"
#include "../event_loop/event_loop.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

String formattedDate;
int hours;
int minutes;

void timer_task(void *parameters) {
    while (1) {
        while(!timeClient.update()) {
            timeClient.forceUpdate();
        }

        formattedDate = timeClient.getFormattedTime();
        hours = timeClient.getHours();
        minutes = timeClient.getMinutes();
        ESP_LOGI(TIMER_LOG_TAG, "current timer : %s", formattedDate);

        event_loop_dispatch(MG_EVENT_TIME_CHANGED, NULL);

        vTaskDelay(10000);
    }
}

current_time get_current_time() {
    return {
        .hours = hours,
        .minutes = minutes,
    };
}

void init_timer(main_config *config) {
    ESP_LOGI(TIMER_LOG_TAG, "initialized");

    timeClient.begin();
    timeClient.setTimeOffset(7200);

    xTaskCreate(timer_task, "timer_task", TIMER_STASK_DEPTH, NULL, 2, NULL);
}