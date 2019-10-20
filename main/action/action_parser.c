#include "cJSON.h"
#include "esp_log.h"

#include "action_parser.h"

mg_config_err parse(const char *raw_action, action_t *output) {
    const cJSON *data = cJSON_Parse(raw_action);

    const cJSON *action = cJSON_GetObjectItemCaseSensitive(data, "action");
    if (cJSON_IsNull(action) || !cJSON_IsNumber(action)) {
        ESP_LOGI(ACTION_PARSER_LOG_TAG, "Invalid json.");
        return MG_ACTION_PARSER_INVALID_ACTION_TYPE;
    }

    const cJSON *value = cJSON_GetObjectItemCaseSensitive(data, "value");

    output->id = action->valueint;
    output->value = value;

    return MG_ACTION_PARSER_OK;
}
