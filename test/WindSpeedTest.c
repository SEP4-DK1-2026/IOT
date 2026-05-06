#include "unity.h"
#include "weathermath.h"

float WindGetKmh(float tips, float seconds);

void setUp() {
}

void tearDown() {
}

void TestWindSpeedNoClick0Seconds(){
    int tips = 0;
    int seconds = 0;
    float kmh = WindGetKmh(tips, seconds);
    TEST_ASSERT_TRUE(isnan(kmh));
}

void TestWindSpeedNoClick5Seconds(){
    int tips = 0;
    int seconds = 5;
    float kmh = WindGetKmh(tips, seconds);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0000f, kmh);
}

void TestWindSpeedNoClick10Seconds(){
    int tips = 0;
    int seconds = 10;
    float kmh = WindGetKmh(tips, seconds);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0000f, kmh);
}

void TestWindSpeedOneClick0Seconds(){
    int tips = 1;
    int seconds = 0;
    float kmh = WindGetKmh(tips, seconds);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0000f, kmh);
}

void TestWindSpeedOneClick5Seconds(){
    int tips = 1;
    int seconds = 5;
    float kmh = WindGetKmh(tips, seconds);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.4800f, kmh);
}

void TestWindSpeedOneClick10Seconds(){
    int tips = 1;
    int seconds = 10;
    float kmh = WindGetKmh(tips, seconds);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.2400f, kmh);
}

void TestWindSpeedFiveClicks0Seconds(){
    int tips = 5;
    int seconds = 0;
    float kmh = WindGetKmh(tips, seconds);
    TEST_ASSERT_FLOAT_IS_INF(kmh);
}

void TestWindSpeedFiveClicks5Seconds(){
    int tips = 5;
    int seconds = 5;
    float kmh = WindGetKmh(tips, seconds);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 2.4000f, kmh);
}

void TestWindSpeedFiveClicks10Seconds(){
    int tips = 5;
    int seconds = 10;
    float kmh = WindGetKmh(tips, seconds);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 1.2000f, kmh);
}

void TestWindSpeedTenClicks0Seconds(){
    int tips = 10;
    int seconds = 0;
    float kmh = WindGetKmh(tips, seconds);
    TEST_ASSERT_FLOAT_IS_INF(kmh);
}

void TestWindSpeedTenClicks5Seconds(){
    int tips = 10;
    int seconds = 5;
    float kmh = WindGetKmh(tips, seconds);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 4.8000f, kmh);
}

void TestWindSpeedTenClicks10Seconds(){
    int tips = 10;
    int seconds = 10;
    float kmh = WindGetKmh(tips, seconds);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 2.4000f, kmh);
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(TestWindSpeedNoClick0Seconds);
    RUN_TEST(TestWindSpeedNoClick5Seconds);
    RUN_TEST(TestWindSpeedNoClick10Seconds);
    RUN_TEST(TestWindSpeedFiveClicks0Seconds);
    RUN_TEST(TestWindSpeedFiveClicks5Seconds);
    RUN_TEST(TestWindSpeedFiveClicks10Seconds);
    RUN_TEST(TestWindSpeedTenClicks0Seconds);
    RUN_TEST(TestWindSpeedTenClicks5Seconds);
    RUN_TEST(TestWindSpeedTenClicks10Seconds);

    return UNITY_END();
}