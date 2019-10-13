#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_log.h"

#include "led.h"
#include "../state/state.h"

#define LED_CHANNEL LEDC_CHANNEL_0
#define LED_FREQ 5000
#define LED_GPIO (GPIO_NUM_19)
#define LED_FADE_UP_TIME 3000
#define LED_FADE_DOWN_TIME 5000

void led_task (void *parameters) {
    while (1) {
        if (get_current_state() != DAYTIME_STATE) {
            ESP_LOGI(LED_LOG_TAG, "NOT DAYTIME");
            vTaskDelay(30000);
            continue;
        }

        ESP_LOGI(LED_LOG_TAG, "ITS DAYTIME");
        ledc_set_fade_with_time(LEDC_HIGH_SPEED_MODE, LED_CHANNEL, (4000), LED_FADE_UP_TIME);
        ledc_fade_start(LEDC_HIGH_SPEED_MODE, LED_CHANNEL, LEDC_FADE_NO_WAIT);

        vTaskDelay(3000);

        ledc_set_fade_with_time(LEDC_HIGH_SPEED_MODE, LED_CHANNEL, 0, LED_FADE_DOWN_TIME);
        ledc_fade_start(LEDC_HIGH_SPEED_MODE, LED_CHANNEL, LEDC_FADE_NO_WAIT);

        vTaskDelay(LED_FADE_DOWN_TIME);
    }
}

void init_led(main_config *config) {
    ESP_LOGI(LED_LOG_TAG, "initialized");

    ledc_timer_config_t ledc_timer = {
        speed_mode: LEDC_HIGH_SPEED_MODE,
        { duty_resolution: LEDC_TIMER_13_BIT, },
        timer_num: LEDC_TIMER_0,
        freq_hz: LED_FREQ,
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        gpio_num: LED_GPIO,
        speed_mode: LEDC_HIGH_SPEED_MODE,
        channel: LED_CHANNEL,
        intr_type: LEDC_INTR_DISABLE,
        timer_sel: LEDC_TIMER_0,
        duty: 0,
        hpoint: 0,
    };
    ledc_channel_config(&ledc_channel);
    ledc_fade_func_install(0);

    xTaskCreate(led_task, "led_task", LED_STASK_DEPTH, NULL, 2, NULL);
}