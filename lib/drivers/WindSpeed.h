#ifndef WIND_SPEED_H
#define WIND_SPEED_H

#include <stdint.h>

void WindSpeed_init(void);
void WindSpeed_reset(void);
unsigned long WindSpeed_getClicks(void);
float WindSpeed_getKmh(float seconds);

#endif