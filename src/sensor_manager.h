#pragma once
#include <stdint.h>

typedef struct {
    uint8_t temp_i;
    uint8_t temp_d;
    uint8_t hum_i;
    uint8_t hum_d;
    uint16_t light;
    float rain;
    float wind_speed;
    uint16_t wind_dir;
    unsigned long wind_clicks;
} sensor_data_t;

void sensors_init(void);
void sensors_read_all(sensor_data_t* data);