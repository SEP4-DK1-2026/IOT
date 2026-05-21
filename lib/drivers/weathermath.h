/***********************************************
 * weathermath.c
 *  Interface for helper functions for testing
 * 
 *  Author:  Sep4DK1
 *  Date:    2026
 *  Project: SPE4_API
 **********************************************/
#pragma once
#include <stdint.h>

float RainGetMM(int tips);
float WindGetKmh(float tips, float seconds);
float LightGetLux(uint16_t rawValue);