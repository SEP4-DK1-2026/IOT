#include "sensor_manager.h"
#include "dht11.h"
#include "light.h"
#include "RainGauge.h"
#include "WindSpeed.h"
#include "WindDir.h"
#include <stdio.h>
#include "SharedWeather.h"
#include <avr/io.h>

#define WIND_MEASUREMENT_SECONDS 16
void sensors_init(void)
{
    light_init();
    RainGauge_init();
    WindSpeed_init();
    WindDir_init();
}

void sensors_read_all(sensor_data_t *data)
{
    // Uden denne bliver cache-flaget aldrit resattet?, så vindretningen bliver kun læst en gang, hvorefter den gentages forevigt ggwpnigga
    WindDir_resetCache();

    dht11_get(&data->hum_i, &data->hum_d,
              &data->temp_i, &data->temp_d);

    data->light = light_measure_raw();

    data->rain = RainGauge_getMM();


    data->wind_clicks = WindSpeed_getClicks();
    data->wind_speed = WindSpeed_getKmh(WIND_MEASUREMENT_SECONDS);

    data->wind_dir = WindDir_getDeg();

    // i debug: convert ADC -> voltage (mV) — use cached ADC (single sample per cycle) (slet når færdig)
    uint16_t adc = WindDir_getCachedADC();
    uint32_t mv = (uint32_t)adc * 5000UL / 1023UL;
    printf("[DEBUG] ADC=%u, mV=%lu\n", adc, mv);
}