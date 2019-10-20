#ifndef MG_ERRORS_H_
#define MG_ERRORS_H_

typedef enum {
    MG_CONFIG_OK = 1,
    MG_CONFIG_WIFI_NOT_CONFIG,
    MG_ACTION_PARSER_OK,
    MG_ACTION_PARSER_INVALID_ACTION_TYPE,
} mg_config_err;

#endif