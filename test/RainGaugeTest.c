#include "unity.h"
#include "weathermath.h"

float RainGetMM(int tips);

void setUp() {
}

void tearDown() {
}

void TestRainGaugeNoClick(){
    int tips = 0;
    float mm = RainGetMM(tips);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0000f, mm);
}

void TestRainGaugeOneClick(){
    int tips = 1;
    float mm = RainGetMM(tips);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.2794f, mm);
}

void TestRainGaugeFiveClicks(){
    int tips = 5;
    float mm = RainGetMM(tips);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 1.3970f, mm);
}

void TestRainGaugeTenClicks(){
    int tips = 10;
    float mm = RainGetMM(tips);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 2.7940f, mm);
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(TestRainGaugeNoClick);
    RUN_TEST(TestRainGaugeOneClick);
    RUN_TEST(TestRainGaugeFiveClicks);
    RUN_TEST(TestRainGaugeTenClicks);

    return UNITY_END();
}