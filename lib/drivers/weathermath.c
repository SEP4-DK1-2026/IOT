#include "weathermath.h"

float RainGetMM(int tips){
    return tips * 0.2794f;
}

float WindGetKmh(float tips, float seconds){
    return (tips / seconds) * 2.4f;   
}