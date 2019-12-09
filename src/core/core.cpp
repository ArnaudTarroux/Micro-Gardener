#include "core.h"

using namespace config;
using namespace microgardener;

void _start_sta(WifiConfig *_wifiConfig);

core_err_t start_core()
{
    Config _config;

    /**
     * Initialize the config and plugins
     */
    WifiConfig wifiPlugin;
    _config.addPlugin(&wifiPlugin);
    config_err_t configErr = _config.begin();

    if (configErr == CONFIG_WIFI_NOT_SETUP) {
        Wifi _wifi(AP_SSID, AP_PASSWORD);
        _wifi.beginAP();
        return CORE_WIFI_AP;
    }

    _start_sta(&wifiPlugin);

    return CORE_OK;
}

void _start_sta(WifiConfig *wifiConfig)
{
    char ssid[MAX_KEY_CONFIG_SIZE];
    char password[MAX_KEY_CONFIG_SIZE];
    wifiConfig->getCredentials(ssid, password);
        
    Wifi _wifi(ssid, password);
    _wifi.beginSTA();
}

