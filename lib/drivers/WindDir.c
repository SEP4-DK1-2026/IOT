/***********************************************
 * WindDir.c
 *  Wind direction sensor implementation
 *
 *  Author:  Sep4DK1
 *  Date:    2026
 *  Project: SPE4_API
 **********************************************/

#include "WindDir.h"
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>

#define WINDDIR_PK_CHANNEL 2
#define WINDDIR_COUNT 16

#define WINDDIR_ADC_MIN_VALID 70
#define WINDDIR_ADC_MAX_VALID 970
#define WINDDIR_ADC_STUCK_HIGH 1015
#define WINDDIR_ADC_STUCK_LOW 3
#define WINDDIR_FILTER_SAMPLES 7
#define WINDDIR_HYSTERESIS 0

/* Internal helper functions */

// Forward declarations for internal helpers
static uint16_t absDiffU16(uint16_t a, uint16_t b);
static uint8_t WindDir_findNearestIndex(uint16_t adc);
static uint8_t WindDir_getIndex(void);

/* ADC values for each wind direction */
static const uint16_t windDirADC[WINDDIR_COUNT] = {
    238, // 0°
    617, // 22.5°
    562, // 45°
    940, // 67.5°
    930, // 90°
    957, // 112.5°
    838, // 135°
    896, // 157.5°
    736, // 180°
    779, // 202.5°
    393, // 225°
    424, // 247.5°
    79,  // 270°
    196, // 292.5°
    136, // 315°
    321  // 337.5°
};

/* Cached values */
static uint16_t cached_adc = 0;
static uint8_t cached_index = 0;
static uint8_t cache_valid = 0;

/* Select ADC channel */
static void WindDir_selectChannel(void)
{
    ADCSRB |= (1 << MUX5);
    ADMUX = (ADMUX & 0xE0) | (WINDDIR_PK_CHANNEL & 0x07);
}

void WindDir_init(void)
{
    /* Set PK2 as analog input */
    DDRK &= ~(1 << PK2);

    /* Disable pull-up resistor */
    PORTK &= ~(1 << PK2);

    /* Use AVCC as reference */
    ADMUX = (1 << REFS0);

    /* Disable digital input on ADC10 */
    DIDR2 |= (1 << ADC10D);

    /* Reset MUX5 */
    ADCSRB &= ~(1 << MUX5);

    /* Enable ADC with prescaler 128 */
    ADCSRA =
        (1 << ADEN) |
        (1 << ADPS2) |
        (1 << ADPS1) |
        (1 << ADPS0);
}

uint16_t WindDir_getDeg(void)
{
    /* Convert direction index to degrees */
    return (uint16_t)((WindDir_getIndex() * 360UL) / WINDDIR_COUNT);
}

void WindDir_resetCache(void)
{
    /* Clear cached value */
    cache_valid = 0;
}

/* Get absolute difference between two values */
static uint16_t absDiffU16(uint16_t a, uint16_t b)
{
    return (a > b) ? (a - b) : (b - a);
}

/* Find closest direction index */
static uint8_t WindDir_findNearestIndex(uint16_t adc)
{
    uint16_t bestDiff = 0xFFFF;
    uint8_t bestIndex = 0;

    for (uint8_t i = 0; i < WINDDIR_COUNT; i++)
    {
        uint16_t diff = absDiffU16(adc, windDirADC[i]);
        if (diff < bestDiff)
        {
            bestDiff = diff;
            bestIndex = i;
        }
    }
    return bestIndex;
}

uint16_t WindDir_getADC(void)
{
    /* Select ADC channel */
    WindDir_selectChannel();

    /* Let ADC settle */
    _delay_us(20);
    /* Discard first ADC reading */
    ADCSRA |= (1 << ADSC);

    while (ADCSRA & (1 << ADSC))
    {
    }

    uint32_t sum = 0;

    /* Average multiple ADC samples */
    for (uint8_t i = 0; i < WINDDIR_FILTER_SAMPLES; i++)
    {
        ADCSRA |= (1 << ADSC);
        while (ADCSRA & (1 << ADSC))
        {
        }
        sum += ADC;
        _delay_us(120);
    }

    return (uint16_t)((sum + (WINDDIR_FILTER_SAMPLES / 2)) / WINDDIR_FILTER_SAMPLES);
}

uint8_t WindDir_getIndex(void)
{

    /* Only read ADC if cache is invalid */
    if (!cache_valid)
    {
        uint16_t adc = WindDir_getADC();
        cached_adc = adc;

        /* Detect obvious electrical fault (open wire, wrong channel, no GND, short) */
        if (adc >= WINDDIR_ADC_STUCK_HIGH || adc <= WINDDIR_ADC_STUCK_LOW)
        {
            cache_valid = 1;
            return cached_index;
        }

        /* Clamp to calibrated span before nearest-match*/
        if (adc < WINDDIR_ADC_MIN_VALID)
            adc = WINDDIR_ADC_MIN_VALID;
        if (adc > WINDDIR_ADC_MAX_VALID)
            adc = WINDDIR_ADC_MAX_VALID;

        uint8_t newIndex = WindDir_findNearestIndex(adc);

        // Small hysteresis so it does not jump between adjacent sectors from noise
        uint16_t prevDiff = absDiffU16(adc, windDirADC[cached_index]);
        uint16_t newDiff = absDiffU16(adc, windDirADC[newIndex]);
        if ((newDiff + WINDDIR_HYSTERESIS) >= prevDiff)
        {
            newIndex = cached_index;
        }

        cached_index = newIndex;
        cache_valid = 1;
    }

    /* Return cached direction */
    return cached_index;
}