/***********************************************
 *  Main.c
 *  Main function of the project 
 * 
 *  Author:  Sep4DK1
 *  Date:    2026
 *  Project: SPE4_API
 **********************************************/

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

    sensors_init();
    network_init();
    sleep_timer_init();

    //_delay_ms(1500000);

    sensor_data_t data;

    while (1)
    {
        sensors_read_all(&data);

    
        send_sensor_data(&data);

        sleep_interval();
    }
}