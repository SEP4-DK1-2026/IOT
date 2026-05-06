#pragma once
#include <stdint.h>

float RainGetMM(int tips);
float WindGetKmh(float tips, float seconds);
float LightGetLux(uint16_t rawValue);