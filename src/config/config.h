#ifndef CONFIG_H_
#define CONFIG_H_

#define CONFIG_LOG_TAG "CONFIG"


struct wifi_config {
    char *ap_ssid;
    char *ap_password;
    char *sta_ssid;
    char *sta_password;
    bool is_sta_ready;
};

struct temp_config {
    float min_temp;
    float max_temp;
};

struct main_config {
    wifi_config wifi;
    temp_config temp;
};

void set_config(void *key, const void *value);
void config_init(main_config *config);

#endif