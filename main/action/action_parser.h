#ifndef ACTION_PARSER_H_
#define ACTION_PARSER_H_

#include "cJSON.h"
#include "mg_errors.h"

#define ACTION_PARSER_LOG_TAG "ACTION_PARSER"

typedef struct {
    int id;
    const cJSON *value;
} action_t;

mg_config_err parse(const char *raw_action, action_t *output);

#endif