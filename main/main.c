#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "wifi/wifi.h"

void app_main()
{
    printf("Hello YOUUUUUUUUUU!\n");

    init_wifi();
}
