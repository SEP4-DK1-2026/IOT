/***********************************************
 *  Sensor_manager.c
 *  Sensor Data gather
 * 
 *  Author:  Sep4DK1
 *  Date:    2026
 *  Project: Sep4DK1
 **********************************************/
#include "sensor_manager.h"
#include "dht11.h"
#include "light.h"
#include "RainGauge.h"
#include "WindSpeed.h"
#include "WindDir.h"
#include <stdio.h>
#include "SharedWeather.h"
#include <avr/io.h>

#define WIND_MEASUREMENT_SECONDS 3600 // 450 * 8 sek = 3600 sek = 60 min
void sensors_init(void)
{
    light_init();
    RainGauge_init();
    WindSpeed_init();
    WindDir_init();
}

void sensors_read_all(sensor_data_t *data)
{
    WindDir_resetCache();

    dht11_get(&data->hum_i, &data->hum_d,
              &data->temp_i, &data->temp_d);

    data->light = light_measure_lux();

    data->rain = RainGauge_getMM();

    data->wind_speed = WindSpeed_getKmh(WIND_MEASUREMENT_SECONDS);

    data->wind_dir = WindDir_getDeg();
}