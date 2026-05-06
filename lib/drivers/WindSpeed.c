#include "WindSpeed.h"
#include "SharedWeather.h"
#include <avr/io.h>
#include <avr/interrupt.h>

//Windspeed og raingauge init er næsten ens kodemæssigt, kan man lave en fælles funktion?
void WindSpeed_init(void)
{
    DDRK &= ~(1 << PK1);
    PORTK |= (1 << PK1);

    /* Ensure digital input enabled on PK1 (ADC9) so PCINT works */
    DIDR2 &= ~(1 << ADC9D);

    /* Sync edge detector with the actual pin level to avoid false startup click. */
    ws_last = (PINK & (1 << PK1)) ? 1 : 0;

    /* Clear pending pin-change interrupt flag before enabling mask. */
    PCIFR |= (1 << PCIF2);

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

float WindSpeed_getKmh(float seconds)
{
    float hz = WindSpeed_getClicks() / seconds;
    float kmh = hz * 2.4f;
    WindSpeed_reset();
    return kmh;
}

