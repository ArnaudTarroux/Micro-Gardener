#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"

#include "config/config.h"
#include "event_source.h"
#include "event_loop/event_loop.h"
#include "action/action_dispatcher.h"
#include "timer/timer.h"
#include "mqtt/mqtt.h"
#include "fan.h"

#define FAN_FREQ 40000
#define DAY_STATE 1
#define NIGHT_STATE 2

static QueueHandle_t duty_cmd;
static float duty_value = 60;
static float duty_night_value = 40;
static int state = DAY_STATE;

void fan_task(void *parameters);
void on_duty_updated(void* handler_args, esp_event_base_t base, int32_t event, void* event_data);
static void on_state_day(void* handler_args, esp_event_base_t base, int32_t event, void* event_data);
static void on_state_night(void* handler_args, esp_event_base_t base, int32_t event, void* event_data);
void send_fan_stats(float actual_duty);

void init_fan() {
    ESP_LOGI(FAN_LOG_TAG, "initialized");

    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, FAN_GPIO);
    mcpwm_config_t pwm_config;
    pwm_config.frequency = FAN_FREQ;
    pwm_config.cmpr_a = 0;
    pwm_config.cmpr_b = 0;
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);

    duty_cmd = xQueueCreate(5, sizeof(float));
    action_register(MG_ACTION_UPDATE_FAN_DUTY, on_duty_updated);
    event_loop_register(MG_STATE_DAY, on_state_day);
    event_loop_register(MG_STATE_NIGHT, on_state_night);
    state = get_timer_state();

    xTaskCreate(fan_task, "fan_task", FAN_STASK_DEPTH, NULL, 2, NULL);

    xQueueSend(duty_cmd, &duty_value, 0);
}

void fan_task(void *parameters) {
    float duty;

    while (1) {
        if (!xQueueReceive(duty_cmd, &duty, 1000 / portTICK_PERIOD_MS)) {
            continue;
        }


        ESP_LOGI(FAN_LOG_TAG, "command received %f", duty);
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, duty);
        send_fan_stats(duty);
    }
}

void on_duty_updated(void* handler_args, esp_event_base_t base, int32_t event, void* event_data) {
    action_t *action = (struct action_t*)event_data;
    cJSON *day = cJSON_GetObjectItem(action->value, "day");
    cJSON *night = cJSON_GetObjectItem(action->value, "night");
    if (!cJSON_IsNumber(day) || !cJSON_IsNumber(night)) {
        ESP_LOGI(TIMER_LOG_TAG, "Invalid value for day or night");
    }

    duty_value = day->valuedouble;
    duty_night_value = night->valuedouble;

    ESP_LOGI(FAN_LOG_TAG, "Actual state: %d", state);

    if (state == DAY_STATE) {
        xQueueSend(duty_cmd, &duty_value, 0);
    } else {
        xQueueSend(duty_cmd, &duty_night_value, 0);
    }
    
    ESP_LOGI(FAN_LOG_TAG, "action id %d, value: %f", action->id, duty_value);
}

void on_state_day(void* handler_args, esp_event_base_t base, int32_t event, void* event_data) {
    ESP_LOGI(FAN_LOG_TAG, "MG_STATE_DAY started");
    state = DAY_STATE;
    xQueueSend(duty_cmd, &duty_value, 0);
}

void on_state_night(void* handler_args, esp_event_base_t base, int32_t event, void* event_data) {
    ESP_LOGI(FAN_LOG_TAG, "MG_STATE_DAY ended");
    state = NIGHT_STATE;
    xQueueSend(duty_cmd, &duty_night_value, 0);
}

void send_fan_stats(float actual_duty) {
    char message[15];
    sprintf(message, "{\"duty\": %2.2f}", actual_duty);
    ESP_LOGI(FAN_LOG_TAG, "Sending :: %s", message);
    publish_message("mg/stats/fan", message);
}