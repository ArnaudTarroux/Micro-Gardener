#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_event.h"
#include "esp_log.h"
#include "event_source.h"
#include "event_loop.h"

ESP_EVENT_DEFINE_BASE(MG_EVENTS);

esp_event_loop_handle_t mg_events_loop;

void event_loop_dispatch(mg_event event, void* event_data) {
    esp_event_post_to(mg_events_loop, MG_EVENTS, event, event_data, sizeof(event_data), portMAX_DELAY);
}

void event_loop_register(mg_event event, esp_event_handler_t handler) {
    esp_event_handler_register_with(mg_events_loop, MG_EVENTS, event, handler, NULL);
}

void init_event_loop() {
    ESP_LOGI(EVENT_LOOP_LOG_TAG, "initialized");

    esp_event_loop_args_t mg_events_loop_args = {
        .queue_size = 10,
        .task_name = "mg_event_task",
        .task_priority = uxTaskPriorityGet(NULL),
        .task_stack_size = 2048,
        .task_core_id = tskNO_AFFINITY
    };

    esp_event_loop_create(&mg_events_loop_args, &mg_events_loop);
}