/***********************************************
 * WindSpeed.c
 *  Wind speed sensor implementation
 *
 *  Author:  Sep4DK1
 *  Date:    2026
 *  Project: SPE4_API
 **********************************************/

#include "WindSpeed.h"
#include "SharedWeather.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "weathermath.h"

float WindGetKmh(float tips, float seconds);


void WindSpeed_init(void)
{
    /* Set PK1 as input */
    DDRK &= ~(1 << PK1);

    /* Enable pull-up resistor */
    PORTK |= (1 << PK1);

    /* Ensure digital input enabled on PK1 (ADC9) so PCINT works */
    DIDR2 &= ~(1 << ADC9D);

    /* Sync edge detector with the actual pin level to avoid false startup click. */
    ws_last = (PINK & (1 << PK1)) ? 1 : 0;

    /* Clear pending pin-change interrupt flag before enabling mask. */
    PCIFR |= (1 << PCIF2);

    /* Enable pin change interrupt */
    PCICR |= (1 << PCIE2);

    /* Enable interrupt on PK1 */
    PCMSK2 |= (1 << PCINT17);
}

void WindSpeed_reset(void)
{
    /* Reset click counter safely */
    cli();
    ws_clicks = 0;
    sei();
}

unsigned long WindSpeed_getClicks(void)
{
    unsigned long c;

    /* Read shared variable safely */
    cli();
    c = ws_clicks;
    sei();

    return c;
}

float WindSpeed_getKmh(float seconds)
{
    /* Convert clicks to km/h */
    float kmh = WindGetKmh(WindSpeed_getClicks(), seconds);

    /* Reset counter after reading */
    WindSpeed_reset();
    return kmh;
}
