#ifndef WIND_SPEED_H
#define WIND_SPEED_H

#include <stdint.h>

void WindSpeed_init(void);
float WindSpeed_getKmh(float seconds);
#endif