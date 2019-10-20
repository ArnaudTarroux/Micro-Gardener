#include "esp_log.h"
#include "esp_event_base.h"

#include "event_source.h"
#include "action_parser.h"
#include "action_dispatcher.h"

ESP_EVENT_DEFINE_BASE(MG_ACTIONS);

esp_event_loop_handle_t mg_actions_loop;

void do_action_dispatch(action_t action_obj);

void action_dispatch(char *data) {
    ESP_LOGI(ACTION_D_LOG_TAG, "Action received %s", data);

    action_t action_obj;
    mg_config_err err = parse(data, &action_obj);
    if (err == MG_ACTION_PARSER_INVALID_ACTION_TYPE) {
        ESP_LOGI(ACTION_D_LOG_TAG, "Wrong type");
        return;
    }

    do_action_dispatch(action_obj);
}

void do_action_dispatch(action_t action_obj) {
    esp_event_post_to(mg_actions_loop, MG_ACTIONS, action_obj.id, &action_obj, sizeof(action_obj), portMAX_DELAY);
}

void action_register(actions action, esp_event_handler_t handler) {
    esp_event_handler_register_with(mg_actions_loop, MG_ACTIONS, action, handler, NULL);
}

void init_action_dispatcher() {
    ESP_LOGI(ACTION_D_LOG_TAG, "initialized");

    esp_event_loop_args_t mg_action_dispatcher_args = {
        .queue_size = 10,
        .task_name = "mg_action_dispatcher_task",
        .task_priority = uxTaskPriorityGet(NULL),
        .task_stack_size = 4096,
        .task_core_id = tskNO_AFFINITY
    };

    esp_event_loop_create(&mg_action_dispatcher_args, &mg_actions_loop);
}