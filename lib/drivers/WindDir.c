#include "WindDir.h"
#include <avr/io.h>
#include <stdint.h>

#define WINDDIR_CHANNEL 10
#define WINDDIR_COUNT 16

static const uint16_t windDirADC[WINDDIR_COUNT] = {
    950,  // N
    880,  // NNE
    800,  // NE
    700,  // ENE
    600,  // E
    520,  // ESE
    450,  // SE
    380,  // SSE
    320,  // S
    280,  // SSW
    240,  // SW
    200,  // WSW
    160,  // W
    120,  // WNW
    80,   // NW
    40    // NNW
};

static void WindDir_selectChannel(void)
{
    uint8_t channel = WINDDIR_CHANNEL;

    ADMUX  = (ADMUX & 0xF0) | (channel & 0x0F);

    if (channel > 7)
        ADCSRB |= (1 << MUX5);
    else
        ADCSRB &= ~(1 << MUX5);
}

void WindDir_init(void)
{
    ADMUX = (1 << REFS0);        // AVCC reference

    DIDR2 |= (1 << ADC10D);      // disable digital input

    ADCSRB &= ~(1 << MUX5);      // clean state

    ADCSRA =
        (1 << ADEN)  |           // enable ADC
        (1 << ADPS2) |
        (1 << ADPS1) |
        (1 << ADPS0);            // prescaler 128
}

uint16_t WindDir_getADC(void)
{
    WindDir_selectChannel();

    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));

    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));

    return ADC;
}

uint8_t WindDir_getIndex(void)
{
    uint16_t adc = WindDir_getADC();

    uint16_t bestDiff = 0xFFFF;
    uint8_t bestIndex = 0;

    for (uint8_t i = 0; i < WINDDIR_COUNT; i++)
    {
        uint16_t diff;

        if (adc > windDirADC[i])
            diff = adc - windDirADC[i];
        else
            diff = windDirADC[i] - adc;

        if (diff < bestDiff)
        {
            bestDiff = diff;
            bestIndex = i;
        }
    }

    return bestIndex;
}

uint16_t WindDir_getDeg(void)
{
    return (uint16_t)((WindDir_getIndex() * 360UL) / WINDDIR_COUNT);
}

const char *WindDir_getText(void)
{
    static const char *dirs[WINDDIR_COUNT] = {
        "N","NNE","NE","ENE",
        "E","ESE","SE","SSE",
        "S","SSW","SW","WSW",
        "W","WNW","NW","NNW"
    };

    return dirs[WindDir_getIndex()];
}