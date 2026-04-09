#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <uart_stdio.h>

#include "sensor_manager.h"
#include "network.h"

int main(void)
{
    sei();
    uart_stdio_init(115200);
    printf("System starting...\n");

    sensors_init();
    network_init();

    sensor_data_t data;

    while (1)
    {
        sensors_read_all(&data);

        printf("Temp: %d.%d Hum: %d.%d Light: %d\n",
               data.temp_i, data.temp_d,
               data.hum_i, data.hum_d,
               data.light);

        send_sensor_data(&data);

        _delay_ms(5000);
    }
}
