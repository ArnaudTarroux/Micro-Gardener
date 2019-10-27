#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "lwip/err.h"
#include "lwip/apps/sntp.h"

#include "timer.h"

#define TIMER_TASK_DELAY 60000

time_t now;
struct tm timeinfo = { 0 };
int timer_start_hour;
int timer_start_minutes;

void timer_task(void *parameters);
mg_timer_err setup_sntp();

mg_timer_err init_timer() {
    ESP_LOGI(TIMER_LOG_TAG, "Initalized");
    mg_timer_err err = setup_sntp();
    if (err != MG_TIMER_OK) {
        return err;
    }

    xTaskCreate(timer_task, "timer_task", TIMER_STASK_DEPTH, NULL, 5, NULL);

    return MG_TIMER_OK;
}

mg_timer_err setup_sntp() {
    time(&now);
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();

    int retry = 0;
    const int retry_count = 10;
    while(timeinfo.tm_year < (2016 - 1900) && ++retry < retry_count) {
        ESP_LOGI(TIMER_LOG_TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(&now);
        localtime_r(&now, &timeinfo);
    }

    if (timeinfo.tm_year < (2016 - 1900)) {
        return MG_TIMER_SNTP_NOT_SETUP;
    }

    setenv("TZ", "UTC", 1);
    tzset();
    now = now + (3600 * 2);
    localtime_r(&now, &timeinfo);

    return MG_TIMER_OK;
}

void timer_task(void *parameters) {
    while (1) {
        if (timeinfo.tm_hour != timer_start_hour && timeinfo.tm_min < timer_start_minutes) {
            vTaskDelay(TIMER_TASK_DELAY / portTICK_PERIOD_MS);
            continue;
        }

        vTaskDelay(TIMER_TASK_DELAY / portTICK_PERIOD_MS);
    }
}
