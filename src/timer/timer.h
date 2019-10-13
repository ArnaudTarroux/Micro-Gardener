#ifndef TIMER_H_
#define TIMER_H_

#include "../config/config.h"

#define TIMER_LOG_TAG "TIMER"
#define TIMER_STASK_DEPTH 4096
#define DEBUG_NTPClient 1

struct current_time {
    int hours;
    int minutes;
};

void init_timer(main_config *config);
current_time get_current_time();

#endif