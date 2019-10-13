#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_log.h"

#include "state.h"
#include "../event_source.h"
#include "../event_loop/event_loop.h"
#include "../timer/timer.h"
#include "../config/config.h"

DAY_STATE current_day_state;
static main_config *config_;

void on_time_changed(void* handler_args, esp_event_base_t base, int32_t event, void* event_data) {
    daytime_config daytime_config = config_->daytime;
    ESP_LOGI(STATE_LOG_TAG, "time changed");
    current_time current_time = get_current_time();

    ESP_LOGI(STATE_LOG_TAG, "current time : %d %d", current_time.hours, current_time.minutes);
    
    int current = (current_time.hours * 100) + current_time.minutes;
    int c_d_start = (daytime_config.start.hours * 100) + daytime_config.start.minutes;
    int c_d_end = (daytime_config.end.hours * 100) + daytime_config.end.minutes;

    ESP_LOGI(STATE_LOG_TAG, "current %d c_d_start %d", current, c_d_start);

    if (current >= c_d_start) {
        current_day_state = DAYTIME_STATE;
    } else {
        current_day_state = NIGHTTIME_STATE;
    }

    ESP_LOGI(STATE_LOG_TAG, "current state %d", current_day_state);

    event_loop_dispatch(MG_EVENT_STATE_CHANGED, &current_day_state);
}

DAY_STATE get_current_state() {
    return current_day_state;
}

void init_state(main_config *config) {
    ESP_LOGI(STATE_LOG_TAG, "initialized");

    config_ = config;
    
    event_loop_register(MG_EVENT_TIME_CHANGED, on_time_changed);
}