#include <unity.h>
#include "WindSpeed.h"
#include "SharedWeather.h"

void handlePinChange(uint8_t pins);

static void simulateWindClick(void)
{
    handlePinChange((1 << PK0) | (1 << PK1));
    handlePinChange(1 << PK0);
}

void setUp() {
    ws_clicks = 0;
    ws_last = 1;
}

void tearDown() {

}

//Different Tests

int main() {

}