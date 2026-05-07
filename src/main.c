#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <uart_stdio.h>
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
    uart_stdio_init(115200);
    printf("[SYSTEM] System starting...\n");

    sensors_init();
    network_init();
    sleep_timer_init();

    sensor_data_t data;

    while (1)
    {
        printf("[LOOP] Starting new cycle\n");

        sensors_read_all(&data);
        printf("[LOOP] Sensors read\n");

        char rainnum[10];
        char wind_speed_num[10];
        dtostrf(data.rain, 6, 2, rainnum);
        dtostrf(data.wind_speed, 6, 2, wind_speed_num);
        printf("[SENSOR] Reading - Temp: %d.%dC Humidity: %d.%d Lux: %d Rainfall: %s Windspeed: %s Winddirection: %u \n",
               data.temp_i, data.temp_d,
               data.hum_i, data.hum_d,
               data.light,
               rainnum,
               wind_speed_num,
               data.wind_dir);

        printf("[LOOP] About to send data\n");
        send_sensor_data(&data);
        printf("[LOOP] Data sent\n");

        printf("[LOOP] About to sleep\n");
        sleep_interval();
        printf("[LOOP] Woke up from sleep\n");
    }
}
// Winddirection: %d \n