#ifndef MG_ERRORS_H_
#define MG_ERRORS_H_

typedef enum {
    MG_CONFIG_OK = 1,
    MG_CONFIG_WIFI_NOT_CONFIG,
    MG_ACTION_PARSER_OK,
    MG_ACTION_PARSER_INVALID_ACTION_TYPE,
    MG_CONFIG_TIMER_NOT_CONFIG,
} mg_config_err;

typedef enum {
    MG_TIMER_OK = 1,
    MG_TIMER_SNTP_NOT_SETUP = 2,
    MG_TIMER_NOT_SETUP = 3,
} mg_timer_err;

#endif