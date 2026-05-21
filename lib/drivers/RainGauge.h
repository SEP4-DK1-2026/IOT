/***********************************************
 * RainGauge.h
 *  Rain gauge sensor interface
 *
 *  Author:  Sep4DK1
 *  Date:    2026
 *  Project: SPE4_API
 **********************************************/

#ifndef RAIN_GAUGE_H
#define RAIN_GAUGE_H

#include <stdint.h>

/* Initialize rain gauge sensor */
void RainGauge_init(void);

/* Reset rain tip counter */
void RainGauge_reset(void);

/* Get current number of tips */
unsigned long RainGauge_getTips(void);

/* Get rainfall in millimeters */
float RainGauge_getMM(void);

#endif