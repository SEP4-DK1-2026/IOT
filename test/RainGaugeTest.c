#include <unity.h>
#include "RainGauge.h"
#include "SharedWeather.h"

void handlePinChange(uint8_t pins);

static void simulateRainClick(void)
{
    handlePinChange((1 << PK0) | (1 << PK1));
    handlePinChange((1 << PK1));
}

void setUp() {
    rg_tips = 0;
    ws_clicks = 0;
    rg_last = 1;
    ws_last = 1;
}

void tearDown() {
}

void TestRainGaugeNoClick(){
    TEST_ASSERT_EQUAL_UINT32(0, rg_tips);

    float mm = RainGauge_getMM();
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0000f, mm);
}

void TestRainGaugeOneClick(){
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

void TestRainGaugeTenClicks()
{
    for(int i = 0; i < 10; i++)
    {
        simulateRainClick();
    }

    TEST_ASSERT_EQUAL_UINT32(10, rg_tips);

    float mm = RainGauge_getMM();
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 2.7940f, mm);
}

void TestRainGaugeResetsAfterRead()
{
    simulateRainClick();
    TEST_ASSERT_EQUAL_UINT32(1, rg_tips);

    RainGauge_getMM();
    TEST_ASSERT_EQUAL_UINT32(0, rg_tips);
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(TestRainGaugeNoClick);
    RUN_TEST(TestRainGaugeOneClick);
    RUN_TEST(TestRainGaugeFiveClicks);
    RUN_TEST(TestRainGaugeTenClicks);
    RUN_TEST(TestRainGaugeResetsAfterRead);

    return UNITY_END();
}