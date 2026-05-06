#ifndef WIND_DIR_H
#define WIND_DIR_H

#include <stdint.h>

void WindDir_init(void);
uint16_t WindDir_getADC(void);
uint16_t WindDir_getCachedADC(void);
const char *WindDir_getText(void);
uint16_t WindDir_getDeg(void);
void WindDir_resetCache(void); 


#endif