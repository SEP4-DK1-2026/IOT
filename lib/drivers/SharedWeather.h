#pragma once
#include <stdint.h>

/* shared ISR state */
extern volatile unsigned long rg_tips;
extern volatile unsigned long ws_clicks;

extern volatile uint8_t rg_last;
extern volatile uint8_t ws_last;