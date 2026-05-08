#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "sleep_timer.h"
#include <stdlib.h>

#include "sensor_manager.h"
#include "network.h"
#include "RainGauge.h"
#include "WindSpeed.h"
#include "WindDir.h"

int main(void)
{
    sei();

    sensors_init();
    network_init();
    sleep_timer_init();

    sensor_data_t data;

    while (1)
    {

        sensors_read_all(&data);

        char rainnum[10];
        char wind_speed_num[10];
        dtostrf(data.rain, 6, 2, rainnum);
        dtostrf(data.wind_speed, 6, 2, wind_speed_num);

        send_sensor_data(&data);

        sleep_interval();
    }
}
// Winddirection: %d \n