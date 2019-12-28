#include "led.h"
#include "core/action/action.h"

#define LED_FREQ 5000
#define LED_CHANNEL 0
#define LED_RESOLUTION 8
#define LED_GPIO GPIO_NUM_19

namespace microgardener {
    static QueueHandle_t dim_cmd;
    int8_t Led::dayState = LED_NIGHT;

    void Led::start()
    {
        Serial.println("Led starting");
        Action::registerAction(DAYSTATE_CHANGED, &this->onDayStateChanged);

        ledcSetup(LED_CHANNEL, LED_FREQ, LED_RESOLUTION);
        ledcAttachPin(LED_GPIO, LED_CHANNEL);

        dim_cmd = xQueueCreate(2, sizeof(int));
        ::xTaskCreatePinnedToCore(&this->runTask, "daystate_task", 10000, NULL, 5, NULL, 1);

        for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
            // changing the LED brightness with PWM
            xQueueSend(dim_cmd, &dutyCycle, 0);
            delay(15);
        }

        // decrease the LED brightness
        for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
            // changing the LED brightness with PWM
            xQueueSend(dim_cmd, &dutyCycle, 0);  
            delay(15);
        }
    }

    void Led::runTask(void *data)
    {
        int dim;
        while (1) {
            if (!xQueueReceive(dim_cmd, &dim, 1000 / portTICK_PERIOD_MS)) {
                continue;
            }

            if (Led::dayState == LED_NIGHT && dim > 0) {
                continue;
            }

            ledcWrite(LED_CHANNEL, dim);

            delay(2000);
        }
    }

    void Led::onDayStateChanged(void *payload)
    {
        Serial.print("onDayStateChanged");
    }
}