#include <unity.h>
#include "RainGauge.h"
#include "SharedWeather.h"

void handlePinChange(uint8_t pins);

static void simulateRainClick(void)
{
    handlePinChange(1 << PK0);
    handlePinChange(0);
}

void setUp() {
    rg_tips = 0;
    ws_clicks = 0;
    rg_last = 1;
    ws_last = 1;
}

void tearDown() {
}

void TestRainGaugeOneFlick(){
    simulateRainClick();

    TEST_ASSERT_EQUAL_UINT32(1, rg_tips);

    float mm = RainGauge_getMM();
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.2794f, mm);
}

void TestRainGaugeFiveClicks()
{
    for(int i = 0; i < 5; i++)
    {
        simulateRainClick();
    }

    TEST_ASSERT_EQUAL_UINT32(5, rg_tips);

    float mm = RainGauge_getMM();
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 1.3970f, mm);
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(TestRainGaugeOneFlick);
    RUN_TEST(TestRainGaugeFiveClicks);
    RUN_TEST(TestRainGaugeTenClicks);
    RUN_TEST(TestRainGaugeResetsAfterRead);

    return UNITY_END();
}