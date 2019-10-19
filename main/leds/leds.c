#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

#include "../config/config.h"

#include "event_source.h"
#include "event_loop/event_loop.h"

#include "leds.h"

#define LED_CHANNEL LEDC_CHANNEL_0
#define LED_FREQ 4000
#define LED_FADE_UP_TIME 3000
#define LED_FADE_DOWN_TIME 5000


void led_task(void *parameters);
double leds_calculate_duty();
void on_leds_status(void* handler_args, esp_event_base_t base, int32_t event, void* event_data);

static QueueHandle_t dim_cmd;
static bool stop_force = 0;

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
    mqtt_event_register(MG_MQTT_LEDS_STATUS, on_leds_status);

    int dim;
    get_leds_dim(&dim);
    xQueueSend(dim_cmd, &dim, 0);
}

void led_task(void *parameters) {
    int dim;

    while (1) {
        if (stop_force) {
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LED_CHANNEL, 0);
            continue;
        }

        if (!xQueueReceive(dim_cmd, &dim, 1000 / portTICK_PERIOD_MS)) {
            continue;
        }

        ESP_LOGI(LEDS_LOG_TAG, "command receiveid %d", dim);
        ledc_set_fade_with_time(LEDC_HIGH_SPEED_MODE, LED_CHANNEL, (dim * 10), LED_FADE_UP_TIME);
        ledc_fade_start(LEDC_HIGH_SPEED_MODE, LED_CHANNEL, LEDC_FADE_NO_WAIT);

        // // vTaskDelay(3000);

        // ledc_set_fade_with_time(LEDC_HIGH_SPEED_MODE, LED_CHANNEL, 0, LED_FADE_DOWN_TIME);
        // ledc_fade_start(LEDC_HIGH_SPEED_MODE, LED_CHANNEL, LEDC_FADE_NO_WAIT);

        // vTaskDelay(LED_FADE_DOWN_TIME / portTICK_PERIOD_MS);
    }
}

void on_leds_status(void* handler_args, esp_event_base_t base, int32_t event, void* event_data) {
    int status = *((int*) event_data);
    ESP_LOGI(LEDS_LOG_TAG, "on_led_status %d", status);
    xQueueSend(dim_cmd, &status, 0);
}