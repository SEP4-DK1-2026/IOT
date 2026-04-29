#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <uart_stdio.h>
#include "sleep_timer.h"

#include "sensor_manager.h"
#include "network.h"

int main(void)
{
    sei();
    uart_stdio_init(115200);
    printf("[SYSTEM] System starting...\n");

    sensors_init();
    network_init();
    sleep_timer_init();

    sensor_data_t data;

    while (1)
    {
        sensors_read_all(&data);

        printf("[SENSOR] Reading - Temp: %d.%d°C Humidity: %d.%d%% Light: %d lux\n",
               data.temp_i, data.temp_d,
               data.hum_i, data.hum_d,
               data.light);

        send_sensor_data(&data);

        sleep_interval();
    }
}
