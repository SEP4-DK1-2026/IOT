#include "RainGauge.h"
#include "SharedWeather.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void RainGauge_init(void)
{
    DDRK &= ~(1 << PK0);
    PORTK |= (1 << PK0);

    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT16); 
}

void RainGauge_reset(void)
{
    cli();
    rg_tips = 0;
    sei();
}

unsigned long RainGauge_getTips(void)
{
    unsigned long t;
    cli();
    t = rg_tips;
    sei();
    return t;
}

float RainGauge_getMM(void)
{
    float mm = RainGauge_getTips() * 0.2794f;
    RainGauge_reset();
    return mm;
}