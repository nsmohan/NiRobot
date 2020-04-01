/** 
 *  @file      unittest_LD27MG.cc
 *  @brief     Unittests for LD27MG.cc
 *  @details   Unittests for LD27MG Motor Driver
 *  @author    Nitin Mohan
 *  @date      March 22, 2019
 *  @copyright 2020 - NM Technologies
 */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <gtest/gtest.h>
#include <iostream>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "LD27MG.h"
#include "PCA9685_stub.h"
#include "NMT_stdlib.h"
#include "NMT_log.h"


/*--------------------------------------------------/
/                   Macros                          /
/--------------------------------------------------*/
#define MAX_MOTORS 2
#define LD27MG_FREQ 50

/* @class MyEnvironment
 *  Environment Setup for Test */
class MyEnvironment: public ::testing::Environment
{
public:
  virtual ~MyEnvironment() = default;

  virtual void SetUp() {NMT_log_init((char *)"/tmp/", false);}

  virtual void TearDown() {NMT_log_finish();}
};

/** @class LD27MG_Test_Fixture
 *  Test Fixture for LD27MG Motor */
class LD27MG_Test_Fixture : public ::testing::Test
{
    public:
       bool sim_mode;
       PCA9685_settings hw_settings;
       PCA9685_PWM_CHANNEL channel;
       PCA9685Mocker PCA9685mock;
       NMT_result result;

       LD27MG_Test_Fixture()
       {
           this->hw_settings = {0};
           this->result = OK;
       }
};

/* ---- Start of Tests -------------*/
using namespace testing;
TEST_F(LD27MG_Test_Fixture, VerifyChannelMapping)
{
   /*!
    *  @test Verify Motor/Channel Mapping by calling
    *  LD27MG_get_current_position and also verify calls
    *  are correctly made to PCA9685_getPWM
    */

    /* Initialize Variables */
    hw_settings.freq = LD27MG_FREQ;
    double angle;
    double duty_cycle;

    int motors[MAX_MOTORS][MAX_MOTORS] = {{CAM_HRZN_MTR, CHANNEL_15}, 
                                          {CAM_VERT_MTR, CHANNEL_14}};

    for (int i = 0; i < MAX_MOTORS; i++)
    {
        /* Verify the Correct Channel is passed to PCA9685_getPWM */
        EXPECT_CALL(PCA9685mock, PCA9685_getPWM(_, (PCA9685_PWM_CHANNEL)motors[i][1]))
            .Times(1)
            .WillOnce(Return(OK));
        EXPECT_CALL(PCA9685mock, PCA9685_get_curret_freq())
            .Times(1)
            .WillOnce(Return(LD27MG_FREQ));
        ASSERT_EQ(result, LD27MG_get_current_position((LD27MG_MOTORS)motors[i][0], &angle));
    }
}

TEST_F(LD27MG_Test_Fixture, VerifyGetAngle)
{
   /*!
    *  @test Verify LD27MG_get_angle function
    *  by passing various different duty cycles
    */

    /* Initialize Variables */
    double angle;
    double duty_cycle;
    double precison = 0.001;

    /* Duty Cycles to Set */
    double duty[] = {2.5, 2.87037, 4.35185, 6.2037, 8.05556, 9.16667};

    /* Expected Angles */
    double angles[] = {0, 10, 50, 100, 150, 180};

    for (int i = 0; i < (sizeof(duty)/sizeof(duty[0])); i++)
    {
        duty_cycle = duty[i];
        EXPECT_CALL(PCA9685mock, PCA9685_getPWM(_, _))
            .WillOnce(DoAll(SetArgPointee<0>(duty_cycle), Return(OK)));
        EXPECT_CALL(PCA9685mock, PCA9685_get_curret_freq())
            .Times(1)
            .WillOnce(Return(LD27MG_FREQ));
        ASSERT_EQ(result, LD27MG_get_current_position(CAM_VERT_MTR, &angle));

        /* Verify actual angle matches expected */
        EXPECT_NEAR(angles[i], angle, precison);
    }
}

TEST_F(LD27MG_Test_Fixture, GetCurrentPostionBW)
{
   /*!
    *  @test Verify overall result is NOK if
    *  PCA9685 returns NOK
    */

    /* Initialize Variables */
    double angle;

    /* Set Expected Result */
    result = NOK;

    EXPECT_CALL(PCA9685mock, PCA9685_getPWM(_, _))
        .Times(1)
        .WillOnce(Return(NOK));
    ASSERT_EQ(result, LD27MG_get_current_position(CAM_VERT_MTR, &angle));
}

TEST_F(LD27MG_Test_Fixture, Move_MotorBW)
{
   /*!
    *  @test Verify overall result is NOK if
    *  PCA9685 returns NOK
    */

    /* Initialize Variables */
    double angle = 10;

    /* Set Expected Result */
    result = NOK;

    EXPECT_CALL(PCA9685mock, PCA9685_setPWM(_, _, _))
        .Times(1)
        .WillOnce(Return(NOK));
    EXPECT_CALL(PCA9685mock, PCA9685_get_curret_freq())
        .Times(1)
        .WillOnce(Return(LD27MG_FREQ));
    ASSERT_EQ(result, LD27MG_move_motor(CAM_VERT_MTR, angle));
}

TEST_F(LD27MG_Test_Fixture, Verify_get_duty_cycle)
{
   /*!
    *  @test Verify LD27MG_get_duty_cycle function
    *  by passing various different angles
    */

    /* Initialize Variables */
    hw_settings.freq = LD27MG_FREQ;
    double angle;
    double precison = 0.0001;
    double duty_cycle;

    /* Expected Duty Cycles*/
    double duty[] = {2.5, 2.5, 2.87037, 4.35185, 6.2037, 8.05556, 9.16667, 9.16667};

    /* Angles Passed In */
    double angles[] = {-1, 0, 10, 50, 100, 150, 180, 200};

    for (int i = 0; i < (sizeof(duty)/sizeof(duty[0])); i++)
    {
        duty_cycle = duty[i];
        EXPECT_CALL(PCA9685mock, PCA9685_setPWM(AllOf(Ge(duty_cycle - precison), Le(duty_cycle + 1)), 0, _))
            .Times(1);
        EXPECT_CALL(PCA9685mock, PCA9685_get_curret_freq())
            .Times(1)
            .WillOnce(Return(LD27MG_FREQ));
        ASSERT_EQ(result, LD27MG_move_motor(CAM_VERT_MTR, angles[i]));
    }
}

TEST_F(LD27MG_Test_Fixture, VerifyInit)
{
   /*!
    *  @test Verify Motors are initialized correctly
    */

    /* Initialize Variables */
    hw_settings.freq = LD27MG_FREQ;
    double precison = 0.0001;

    double expected_duty = 5.8333;

    EXPECT_CALL(PCA9685mock, PCA9685_setPWM(AllOf(Ge(expected_duty - precison), Le(expected_duty + 1)), 0, _))
        .Times(2);
    EXPECT_CALL(PCA9685mock, PCA9685_get_init_status(_))
        .WillOnce(DoAll(SetArgPointee<0>(true), Return(OK)));
    EXPECT_CALL(PCA9685mock, PCA9685_get_curret_freq())
        .Times(1)
        .WillOnce(Return(LD27MG_FREQ));
    ASSERT_EQ(result, LD27MG_init());
}

TEST_F(LD27MG_Test_Fixture, InitBW)
{
   /*!
    *  @test Verify overall result is NOK if
    *  PCA9685 returns NOK
    */

    /* Set Expected Result */
    result = NOK;

    EXPECT_CALL(PCA9685mock, PCA9685_setPWM(_, _, _))
        .Times(1)
        .WillOnce(Return(NOK));
    EXPECT_CALL(PCA9685mock, PCA9685_get_init_status(_))
        .WillOnce(DoAll(SetArgPointee<0>(true), Return(OK)));
    EXPECT_CALL(PCA9685mock, PCA9685_get_curret_freq())
        .Times(1)
        .WillOnce(Return(LD27MG_FREQ));
    ASSERT_EQ(result, LD27MG_init());
}

TEST_F(LD27MG_Test_Fixture, InitBW2)
{
   /*!
    *  @test Verify overall result is NOK if
    *  PCA9685 Driver is Not Initialized
    */

    /* Set Expected Result */
    result = NOK;

    EXPECT_CALL(PCA9685mock, PCA9685_setPWM(_, _, _))
        .Times(0);
    EXPECT_CALL(PCA9685mock, PCA9685_get_init_status(_))
        .WillOnce(DoAll(SetArgPointee<0>(false), Return(OK)));
    EXPECT_CALL(PCA9685mock, PCA9685_get_curret_freq())
        .Times(0);
    ASSERT_EQ(result, LD27MG_init());
}

TEST_F(LD27MG_Test_Fixture, InitBW3)
{
   /*!
    *  @test Verify overall result is NOK if
    *  PCA9685_get_init_status is NOK
    */

    /* Set Expected Result */
    result = NOK;

    EXPECT_CALL(PCA9685mock, PCA9685_setPWM(_, _, _))
        .Times(0);
    EXPECT_CALL(PCA9685mock, PCA9685_get_init_status(_))
        .WillOnce(DoAll(SetArgPointee<0>(true), Return(NOK)));
    EXPECT_CALL(PCA9685mock, PCA9685_get_curret_freq())
        .Times(0);
    ASSERT_EQ(result, LD27MG_init());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    MyEnvironment* env = new MyEnvironment(); 
    ::testing::AddGlobalTestEnvironment(env);
    return RUN_ALL_TESTS();
}

