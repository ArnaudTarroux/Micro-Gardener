#ifndef DAY_STATE_H_
#define DAY_STATE_H_

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "sntp/sntp.h"

#include "core/action/action.h"

using namespace action;

namespace microgardener {
    class DayState {
    public:
        virtual void start(Sntp sntp);
    private:
        static void runTask(void *data);
    };
}

#endif