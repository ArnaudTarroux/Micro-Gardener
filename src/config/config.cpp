#include <esp_log.h>
#include "string.h"
#include "config.h"
#include "../event_source.h"
#include "../event_loop/event_loop.h"

main_config *config_;

void config_init(main_config *config) {
    config_ = config;
}

void set_daytime_config(daytime_config daytime_config) {
    config_->daytime = daytime_config;
}