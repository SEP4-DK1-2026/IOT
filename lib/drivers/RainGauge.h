#ifndef RAIN_GAUGE_H
#define RAIN_GAUGE_H

#include <stdint.h>

void RainGauge_init(void);
void RainGauge_reset(void);
unsigned long RainGauge_getTips(void);
float RainGauge_getMM(void);

#endif