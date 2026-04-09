#include "sensor_manager.h"
#include "dht11.h"
#include "light.h"

void sensors_init(void)
{
    light_init();
}

void sensors_read_all(sensor_data_t* data)
{
    dht11_get(&data->hum_i, &data->hum_d,
              &data->temp_i, &data->temp_d);

    data->light = light_measure_raw();
}