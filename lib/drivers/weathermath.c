#include "weathermath.h"
#include <stdint.h>
#include <math.h>

float RainGetMM(int tips){
    return tips * 0.2794f;
}

float WindGetKmh(float tips, float seconds){
    return (tips / seconds) * 2.4f;   
}

float LightGetLux(uint16_t rawValue){

    if(rawValue > 1023)
    {
       rawValue = 1023; //ADC max value
    }

    if(rawValue == 0)
    {
        rawValue = 1; //Will probably never be 0, bust just in case. You can never divide by 0
    }

    float voltage = rawValue * 5.0f / 1023.0f;

    if(voltage == 5.0f)
    {
        voltage = 4.995f;
    }

    float rldr = (5.0f - voltage) * 10000.0f / voltage;

    float lux = powf((500000.0f / rldr), 1.4f);
    return lux;
}