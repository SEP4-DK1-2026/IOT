/***********************************************
 * WindSpeed.h
 *  Wind speed sensor interface
 * 
 *  Author:  Sep4DK1
 *  Date:    2026
 *  Project: SPE4_API
 **********************************************/

#ifndef WIND_SPEED_H
#define WIND_SPEED_H

#include <stdint.h>
/* Initialize wind speed sensor */
void WindSpeed_init(void);

/* Get wind speed in km/h */
float WindSpeed_getKmh(float seconds);

#endif