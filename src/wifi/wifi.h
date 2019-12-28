#ifndef WIFI_H_
#define WIFI_H_

#include <WiFi.h>

#define AP_SSID "Microgardener"
#define AP_PASSWORD "wannagrow"

namespace microgardener {
    class Wifi {
    private:
        char *ssid, *password;

    public:
        Wifi(char *ssid, char *password);
        void beginAP();
        void beginSTA(int maxRetries=10);
    };
}

#endif