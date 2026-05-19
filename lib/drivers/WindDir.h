/***********************************************
 * WindDir.h
 *  Wind direction sensor interface
 * 
 *  Author:  Sep4DK1
 *  Date:    2026
 *  Project: SPE4_API
 **********************************************/
#ifndef WIND_DIR_H
#define WIND_DIR_H

#include <stdint.h>
/* Initialize wind direction sensor */
void WindDir_init(void);

/* Get raw ADC value */
uint16_t WindDir_getADC(void);

/* Get wind direction in degrees */
uint16_t WindDir_getDeg(void);

/* Reset cached direction value */
void WindDir_resetCache(void); 

#endif