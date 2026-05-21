/***********************************************
 * RainGauge.c
 *  Rain gauge sensor implementation
 *
 *  Author:  SEP4DK1
 *  Date:    2026
 *  Project: SPE4_API
 **********************************************/

#include "RainGauge.h"
#include "SharedWeather.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "weathermath.h"

float RainGetMM(int tips);

void RainGauge_init(void)
{
    /* Set PK0 as input */
    DDRK &= ~(1 << PK0);
    /* Enable pull-up resistor */
    PORTK |= (1 << PK0);

    /* Enable digital input on PK0 */
    DIDR2 &= ~(1 << ADC8D);

    /* Read current pin state to avoid false startup trigger */
    rg_last = (PINK & (1 << PK0)) ? 1 : 0;

    /* Clear pending interrupt flag */
    PCIFR |= (1 << PCIF2);
    /* Enable pin change interrupt */
    PCICR |= (1 << PCIE2);

    /* Enable interrupt on PK0 */
    PCMSK2 |= (1 << PCINT16);
}

void RainGauge_reset(void)
{
    /* Disable interrupts while resetting counter */
    cli();
    rg_tips = 0;
    sei();
}

unsigned long RainGauge_getTips(void)
{
    unsigned long t;

    /* Read shared variable safely */
    cli();
    t = rg_tips;
    sei();
    return t;
}

float RainGauge_getMM(void)
{
    /* Convert tips to millimeters */
    float mm = RainGetMM(RainGauge_getTips());

    /* Reset counter after reading */
    RainGauge_reset();

    return mm;
}