#include "string.h"
#include "config.h"

void set_config(void *key, const void *value) {
    memccpy(key, value, 0, sizeof(key));
}