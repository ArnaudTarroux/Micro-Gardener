#ifndef CLIMATE_H_
#define CLIMATE_H_

#include <Arduino.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_AM2320.h"

namespace microgardener {
    class Climate {
    private:
        Adafruit_AM2320 am2320 = Adafruit_AM2320();
    public:
        Climate();
        float getTemperature();
        float getHumidity();
    };
}

#endif