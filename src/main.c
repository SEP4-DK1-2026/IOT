#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <uart_stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sensor_manager.h"
#include "network.h"
#include "RainGauge.h"
#include "WindSpeed.h"
#include "WindDir.h"

int main(void)
{
    sei();
    uart_stdio_init(115200);
    printf("System starting...\n");

    sensors_init();
    network_init();
    RainGauge_init();
    WindSpeed_init();
    WindDir_init();

    sensor_data_t data;

    while (1)
    {
        sensors_read_all(&data);
  char rainnum[10];
    char wind_speed_num[10];
    time_t seconds;
    ;
dtostrf(data.rain, 6, 2,rainnum);
        printf("Temp: %d.%d Hum: %d.%d Light: %d Rainfall: %s Windspeed: %s Winddirriction: %d time: %ld\n",
               data.temp_i, data.temp_d,
               data.hum_i, data.hum_d,
               data.light,
               rainnum,
              wind_speed_num,
               data.wind_dir,
            time(&seconds));

        send_sensor_data(&data);

        _delay_ms(5000);
    }
}
