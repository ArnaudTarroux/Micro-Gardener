#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "daystate.h"

namespace microgardener {

    struct daystate_taskData_t {
        Sntp sntp;
    };

    void DayState::start(Sntp sntp)
    {
        Serial.println("DayState starting");
        
        daystate_taskData_t taskData = {
            sntp: sntp,
        };
        ::xTaskCreatePinnedToCore(&this->runTask, "daystate_task", 10000, &taskData, 5, NULL, 1);
    }

    void DayState::runTask(void *data)
    {
        daystate_taskData_t *taskData = static_cast<daystate_taskData_t *>(data);
        Sntp sntp = taskData->sntp; 

        int16_t start = 1700;
        int16_t stop = 017;

        while (1) {
            int16_t currentCalculatedHour = (sntp.getHours()*100) + sntp.getMinutes();
            Serial.printf("currentCalculatedHour %d \n", currentCalculatedHour);

            // Si le jour est à cheval sur 2 jours (start: 17h00 stop 05:00)
            if (stop < start) {
                if (currentCalculatedHour >= start && currentCalculatedHour <= 2359) {
                    Serial.println("day");
                    Action::dispatchAction(DAYSTATE_CHANGED, "day");
                } else if (currentCalculatedHour >= 0 && currentCalculatedHour <= stop) {
                    Serial.println("day");
                    Action::dispatchAction(DAYSTATE_CHANGED, "day");
                } else {
                    Serial.println("night");
                    Action::dispatchAction(DAYSTATE_CHANGED, "night");
                }
            } else {
                 if (currentCalculatedHour >= start && currentCalculatedHour <= stop) {
                    Serial.println("day");
                    Action::dispatchAction(DAYSTATE_CHANGED, "day");
                } else {
                    Serial.println("night");
                    Action::dispatchAction(DAYSTATE_CHANGED, "night");
                }
            }

            delay(60000);
        }
    }
}