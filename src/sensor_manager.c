#include "sensor_manager.h"
#include "dht11.h"
#include "light.h"
#include "RainGauge.h"
#include "WindSpeed.h"
#include "WindDir.h"

void sensors_init(void)
{
    light_init();
}

void sensors_read_all(sensor_data_t* data)
{
    dht11_get(&data->hum_i, &data->hum_d,
              &data->temp_i, &data->temp_d);

    data->light = light_measure_raw();

    data->rain = RainGauge_getMM();

    data->wind_speed = WindSpeed_getKmh(5.0f);

    data->wind_dir = WindDir_getDeg();

}