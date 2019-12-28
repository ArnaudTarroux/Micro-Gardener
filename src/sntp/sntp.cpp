#include "sntp.h"

namespace microgardener {
    Sntp::Sntp(int16_t offset)
    {
        this->offset = offset;
    }

    void Sntp::begin()
    {
        this->timeClient.begin();
        this->timeClient.setTimeOffset(offset);

        while(!timeClient.update()) {
            timeClient.forceUpdate();
        }
    }

    int Sntp::getHours()
    {
        return this->timeClient.getHours();
    }

    int Sntp::getMinutes()
    {
        return this->timeClient.getMinutes();
    }
    
    int Sntp::getSeconds()
    {
        return this->timeClient.getSeconds();
    }
}