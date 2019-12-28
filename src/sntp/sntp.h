#ifndef SNTP_H_
#define SNTP_H_

#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

namespace microgardener {
    class Sntp {
    private:
        int16_t offset;
        WiFiUDP ntpUDP;
        NTPClient timeClient = NTPClient(this->ntpUDP, this->offset);

    public:
        Sntp(int16_t offset);
        void begin();
        int getHours();
        int getMinutes();
        int getSeconds();
    };
}

#endif