#include <Arduino.h>
#include "wifi.h"


namespace microgardener {

    Wifi::Wifi(char *ssid, char *password)
    {
        this->ssid = ssid;
        this->password = password;

        
    }

    void Wifi::beginSTA(int maxRetries)
    {
        WiFi.mode(WIFI_MODE_STA);
        WiFi.begin(this->ssid, this->password);
        int retry = 1;
        while (retry <= maxRetries && WiFi.status() != WL_CONNECTED) {
            Serial.printf("Wifi connexion %d/%d \n", retry, maxRetries);
            Serial.printf("Wifi status %d\n", WiFi.status());
            delay(1000);
            retry ++;
        }

        Serial.printf("Wifi connexion status %d\n", WiFi.status());
    }

    void Wifi::beginAP()
    {
        WiFi.softAP(this->ssid, this->password);
        Serial.print("Wifi AP mode.\nIP: ");
        Serial.print(WiFi.softAPIP());
        Serial.println();
    }
}