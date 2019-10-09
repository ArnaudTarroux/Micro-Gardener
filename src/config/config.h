#ifndef CONFIG_H_
#define CONFIG_H_

struct wifi_config {
    char *ap_ssid;
    char *ap_password;
    char *sta_ssid;
    char *sta_password;
    bool is_sta_ready;
};

struct main_config {
    wifi_config wifi;
};

void set_config(void *key, const void *value);

#endif