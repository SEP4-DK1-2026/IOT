#include "WindSpeed.h"
#include "SharedWeather.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void WindSpeed_init(void)
{
    DDRK &= ~(1 << PK1);
    PORTK |= (1 << PK1);

    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT17); 
}

void WindSpeed_reset(void)
{
    cli();
    ws_clicks = 0;
    sei();
}

unsigned long WindSpeed_getClicks(void)
{
    unsigned long c;
    cli();
    c = ws_clicks;
    sei();
    return c;
}

float WindSpeed_getMS(float seconds)
{
    float hz = WindSpeed_getClicks() / seconds;
    float ms = hz * 0.6667f;
    WindSpeed_reset();
    return ms;
}