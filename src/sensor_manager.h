/***********************************************
 *  Sensor_manager.h
 *  Sensor Data gather interface
 * 
 *  Author:  Sep4DK1
 *  Date:    2026
 *  Project: Sep4DK1
 **********************************************/
#pragma once
#include <stdint.h>

typedef struct {
    uint8_t temp_i;
    uint8_t temp_d;
    uint8_t hum_i;
    uint8_t hum_d;
    float light;
    float rain;
    float wind_speed;
    uint16_t wind_dir;
} sensor_data_t;

void sensors_init(void);
void sensors_read_all(sensor_data_t* data);