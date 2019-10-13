#ifndef STATE_H_
#define STATE_H_

#include "../config/config.h"

#define STATE_LOG_TAG "STATE"
#define TIMER_STASK_DEPTH 4096

enum DAY_STATE {
    DAYTIME_STATE = 1,
    NIGHTTIME_STATE = 2,
};

void init_state(main_config *config);
DAY_STATE get_current_state();

#endif