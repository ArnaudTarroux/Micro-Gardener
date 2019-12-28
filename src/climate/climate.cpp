#include "climate.h"

namespace microgardener {
    Climate::Climate()
    {
        am2320.begin();
    }

    float Climate::getTemperature()
    {
        return am2320.readTemperature();
    }

    float Climate::getHumidity()
    {
        return am2320.readHumidity();
    }
}