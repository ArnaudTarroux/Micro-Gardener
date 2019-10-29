#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "lwip/err.h"
#include "lwip/apps/sntp.h"
#include "cJSON.h"

#include "timer.h"
#include "config/config.h"
#include "event_source.h"
#include "event_loop/event_loop.h"
#include "action/action_dispatcher.h"

#define TIMER_TASK_DELAY 60000

time_t now;
struct tm timeinfo = { 0 };
static int timer_start = 530;
static int timer_stop = 1953;
int state;

void timer_task(void *parameters);
void on_update_timer_period(void* handler_args, esp_event_base_t base, int32_t event, void* event_data);
mg_timer_err setup_sntp();

mg_timer_err init_timer() {
    ESP_LOGI(TIMER_LOG_TAG, "Initalized");
    mg_timer_err err = setup_sntp();
    if (err != MG_TIMER_OK) {
        return err;
    }

    mg_config_err timer_err = get_timer_period(&timer_start, &timer_stop);
    if (timer_err == MG_CONFIG_TIMER_NOT_CONFIG) {
        ESP_LOGI(TIMER_LOG_TAG, "Timer not initialized, please set start and stop");
        return MG_TIMER_NOT_SETUP;
    }

    action_register(MG_ACTION_UPDATE_TIMER_PERIOD, on_update_timer_period);
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

    return MG_TIMER_OK;
}

void timer_task(void *parameters) {
    while (1) {
        time(&now);
        now = now + (3600 * 1);
        localtime_r(&now, &timeinfo);
        ESP_LOGI(TIMER_LOG_TAG, "CURRENT TIME %d:%d    end at %d", timeinfo.tm_hour, timeinfo.tm_min, timer_stop);

        if (((timeinfo.tm_hour * 100 + timeinfo.tm_min) >= timer_start) &&
            ((timeinfo.tm_hour * 100 + timeinfo.tm_min) <= timer_stop)) {

            if (state != TIMER_STATE_DAY) {
                state = TIMER_STATE_DAY;
                event_loop_dispatch(MG_STATE_DAY, NULL);
                ESP_LOGI(TIMER_LOG_TAG, "DAY STARTED %d -> %d:%d", state, timeinfo.tm_hour, timeinfo.tm_min);
            }

            vTaskDelay(TIMER_TASK_DELAY / portTICK_PERIOD_MS);
            continue;
        }

        if (state != TIMER_STATE_NIGHT) {
            state = TIMER_STATE_NIGHT;
            event_loop_dispatch(MG_STATE_NIGHT, NULL);
            ESP_LOGI(TIMER_LOG_TAG, "DAY ENDED %d -> %d:%d", state, timeinfo.tm_hour, timeinfo.tm_min);
        }

        vTaskDelay(TIMER_TASK_DELAY / portTICK_PERIOD_MS);
    }
}

int get_timer_state() {
    return state;
}

void on_update_timer_period(void* handler_args, esp_event_base_t base, int32_t event, void* event_data) {
    action_t *action = (struct action_t*)event_data;
    if (!cJSON_IsObject(action->value)) {
        ESP_LOGI(TIMER_LOG_TAG, "Invalid value for action %d", action->id);
        return;
    }

    cJSON *start = cJSON_GetObjectItem(action->value, "start");
    cJSON *stop = cJSON_GetObjectItem(action->value, "stop");
    if (!cJSON_IsNumber(start) || !cJSON_IsNumber(stop)) {
        ESP_LOGI(TIMER_LOG_TAG, "Invalid value for start or stop");
    }
   
   ESP_LOGI(TIMER_LOG_TAG, "New timer period %d -> %d", start->valueint, stop->valueint);
   set_timer_period(start->valueint, stop->valueint);
   timer_start = start->valueint;
   timer_stop = stop->valueint;
}