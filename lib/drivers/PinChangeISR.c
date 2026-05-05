#include "SharedWeather.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/* shared state */
volatile unsigned long rg_tips = 0;
volatile unsigned long ws_clicks = 0;

volatile uint8_t rg_last = 1;
volatile uint8_t ws_last = 1;

ISR(PCINT2_vect)
{
    handlePinChange(PINK);
}

void handlePinChange(uint8_t pins)
{
    uint8_t r = (pins & (1 << PK0)) ? 1 : 0;
    uint8_t w = (pins & (1 << PK1)) ? 1 : 0;

    /* Rain on falling edge */
    if (rg_last == 1 && r == 0)
        rg_tips++;

    /* Wind speed on falling edge */
    if (ws_last == 1 && w == 0)
        ws_clicks++;

    rg_last = r;
    ws_last = w;
}