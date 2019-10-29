#ifndef TIMER_H_
#define TIMER_H_

#include "mg_errors.h"

#define TIMER_LOG_TAG "TIMER"
#define TIMER_STASK_DEPTH 4096
#define TIMER_STATE_DAY 1
#define TIMER_STATE_NIGHT 2

mg_timer_err init_timer();
int get_timer_state();

#endif