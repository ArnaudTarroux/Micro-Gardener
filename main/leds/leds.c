#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

#include "config/config.h"
#include "event_source.h"
#include "event_loop/event_loop.h"
#include "action/action_dispatcher.h"

#include "leds.h"

#define LED_CHANNEL LEDC_CHANNEL_0
#define LED_FREQ 4000
#define LED_FADE_DURATION 5000


void led_task(void *parameters);
double leds_calculate_duty();
void on_leds_on_off(void* handler_args, esp_event_base_t base, int32_t event, void* event_data);
void on_dim_updated(void* handler_args, esp_event_base_t base, int32_t event, void* event_data);

static QueueHandle_t dim_cmd;

void init_leds() {
    ESP_LOGI(LEDS_LOG_TAG, "initialized");

    ledc_timer_config_t ledc_timer = {
        speed_mode: LEDC_HIGH_SPEED_MODE,
        { duty_resolution: LEDC_TIMER_13_BIT, },
        timer_num: LEDC_TIMER_0,
        freq_hz: LED_FREQ,
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        gpio_num: LEDS_GPIO,
        speed_mode: LEDC_HIGH_SPEED_MODE,
        channel: LED_CHANNEL,
        intr_type: LEDC_INTR_DISABLE,
        timer_sel: LEDC_TIMER_0,
        duty: 0,
        hpoint: 0,
    };
    ledc_channel_config(&ledc_channel);
    ledc_fade_func_install(0);
    
    dim_cmd = xQueueCreate(5, sizeof(int));
    xTaskCreate(led_task, "led_task", LEDS_STASK_DEPTH, NULL, 2, NULL);

    action_register(MG_ACTION_UPDATE_DIM, on_dim_updated);
    action_register(MG_ACTION_LEDS_ON_OFF, on_leds_on_off);

    int dim;
    get_leds_dim(&dim);
    xQueueSend(dim_cmd, &dim, 0);
}

void led_task(void *parameters) {
    int dim;

    while (1) {
        if (!xQueueReceive(dim_cmd, &dim, 1000 / portTICK_PERIOD_MS)) {
            continue;
        }

        ESP_LOGI(LEDS_LOG_TAG, "command receiveid %d", dim);
        ledc_set_fade_with_time(LEDC_HIGH_SPEED_MODE, LED_CHANNEL, (dim * 10), LED_FADE_DURATION);
        ledc_fade_start(LEDC_HIGH_SPEED_MODE, LED_CHANNEL, LEDC_FADE_NO_WAIT);

        vTaskDelay((LED_FADE_DURATION * 1.1) / portTICK_PERIOD_MS);
    }
}

void on_leds_on_off(void* handler_args, esp_event_base_t base, int32_t event, void* event_data) {
    action_t *action = (struct action_t*)event_data;
    if (!cJSON_IsNumber(action->value) && action->value->valueint != 0 && action->value->valueint != 1) {
        ESP_LOGI(LEDS_LOG_TAG, "Invalid value for action %d", action->id);
        return;
    }

    bool status = action->value->valueint;
    int dim = 0;
    if (status == 1) {
        get_leds_dim(&dim);
        xQueueSend(dim_cmd, &dim, 0);
    } else {
        xQueueSend(dim_cmd, &dim, 0);
    }

    ESP_LOGI(LEDS_LOG_TAG, "on_led_status %d", status);
    
}

void on_dim_updated(void* handler_args, esp_event_base_t base, int32_t event, void* event_data) {
    action_t *action = (struct action_t*)event_data;
    if (!cJSON_IsNumber(action->value)) {
        ESP_LOGI(LEDS_LOG_TAG, "Invalid value for action %d", action->id);
        return;
    }

    int dim_value = action->value->valueint;
    xQueueSend(dim_cmd, &dim_value, 0);
    ESP_LOGI(LEDS_LOG_TAG, "action id %d, value: %d", action->id, dim_value);
    set_leds_dim(dim_value);
}