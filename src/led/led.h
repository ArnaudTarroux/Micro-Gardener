#ifndef LED_H_
#define LED_H_

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include "core/action/action.h"

#define LED_DAY 1
#define LED_NIGHT 2

using namespace action;

namespace microgardener {
    class Led {
    public:
        void start();


    private:
        static void runTask(void *data);
        static void onDayStateChanged(void *payload);
        static int8_t dayState;
    };
}

#endif