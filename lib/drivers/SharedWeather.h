/***********************************************
 *  SharedWeather.h
 *  Interface for setup for interrupt pins 
 * 
 *  Author:  Sep4DK1
 *  Date:    2026
 *  Project: SPE4_API
 **********************************************/
#pragma once
#include <stdint.h>

/* shared ISR state */
extern volatile unsigned long rg_tips;
extern volatile unsigned long ws_clicks;

extern volatile uint8_t rg_last;
extern volatile uint8_t ws_last;