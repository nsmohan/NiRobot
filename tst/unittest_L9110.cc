/** 
 *  @file      unittest_L9110.cc
 *  @brief     Unittests for L9110.cc
 *  @details   Unittests for the H-Bridge Module
 *  @author    Nitin Mohan
 *  @date      March 27, 2019
 *  @copyright 2020 - NM Technologies
 */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <gtest/gtest.h>
#include <iostream>
#include <string.h>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "wiringPi_stub.h"
#include "L9110.hpp"
#include "RSXA.h"
#include "NMT_log.h"

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
class L9110_Test_Fixture : public ::testing::Test
{
    public:
       RSXA_hw hw_config;
       wiringPiMocker wpimock;

       L9110_Test_Fixture()
       {
           hw_config = {0};
           strcpy(hw_config.hw_name, "LEFT_DRV_MOTOR");
           hw_config.hw_sim_mode = false;
           hw_config.hw_interface = (RSXA_pins *)malloc(sizeof(RSXA_pins) * 2);
           strcpy(hw_config.hw_interface[0].pin_name, "forward");
           strcpy(hw_config.hw_interface[1].pin_name, "reverse");
           hw_config.hw_interface[0].pin_no = 1;
           hw_config.hw_interface[1].pin_no = 2;
       }
};

/* ---- Start of Tests -------------*/
using namespace testing;
TEST_F(L9110_Test_Fixture, VerifyConstructorGW)
{
   /*!
    *  @test Create the L9110 Object and
    *  verify all calls to init hardware are correct
    *  In Sim Mode and Not in Sim Mode
    */

    /* Scenario 1 - Not in Sim Mode */
    EXPECT_CALL(wpimock, pinMode(AnyOf(1,2), OUTPUT)).Times(2);
    EXPECT_CALL(wpimock, digitalWrite(AnyOf(1,2), 0.00)).Times(AtLeast(2));
    L9110 l9110_obj(hw_config);

    /* Scenario 2 - In Sim Mode */
    hw_config.hw_sim_mode = true;
    EXPECT_CALL(wpimock, pinMode(_, _)).Times(0);
    EXPECT_CALL(wpimock, digitalWrite(_, _)).Times(0);
    L9110 l9110_obj1(hw_config);

}

TEST_F(L9110_Test_Fixture, VerifyConstructorBW)
{
   /*!
    *  @test Verify Constructor throws an exception if
    *  pin name is invalid for forward
    *  In Sim Mode and Not in Sim Mode
    */

    /* Scenario 1 - Not in Sim Mode */
    strcpy(hw_config.hw_interface[0].pin_name, "Test1");
    EXPECT_CALL(wpimock, pinMode(_, _)).Times(0);
    EXPECT_CALL(wpimock, digitalWrite(_, _)).Times(0);

    /* Construct Object and catch Exception */
    try
    {
        L9110 l9110_obj(hw_config);
        FAIL();
    }
    catch (std::exception &e)
    {
        SUCCEED();
    }

    /* Scenario 2 - In Sim Mode */
    hw_config.hw_sim_mode = true;
    EXPECT_CALL(wpimock, pinMode(_, _)).Times(0);
    EXPECT_CALL(wpimock, digitalWrite(_, _)).Times(0);

    /* Construct Object and catch Exception */
    try
    {
        L9110 l9110_obj1(hw_config);
        FAIL();
    }
    catch (std::exception &e2)
    {
        SUCCEED();
    }
}

TEST_F(L9110_Test_Fixture, VerifyConstructor2BW)
{
   /*!
    *  @test Verify Constructor throws an exception if
    *  pin name is invalid for reverse
    *  In Sim Mode and Not in Sim Mode
    */

    /* Scenario 1 - Not in Sim Mode */
    strcpy(hw_config.hw_interface[1].pin_name, "Test2");
    EXPECT_CALL(wpimock, pinMode(_, _)).Times(0);
    EXPECT_CALL(wpimock, digitalWrite(_, _)).Times(0);

    /* Construct Object and catch Exception */
    try
    {
        L9110 l9110_obj(hw_config);
        FAIL();
    }
    catch (std::exception &e)
    {
        SUCCEED();
    }

    /* Scenario 2 - In Sim Mode */
    hw_config.hw_sim_mode = true;
    EXPECT_CALL(wpimock, pinMode(_, _)).Times(0);
    EXPECT_CALL(wpimock, digitalWrite(_, _)).Times(0);

    /* Construct Object and catch Exception */
    try
    {
        L9110 l9110_obj1(hw_config);
        FAIL();
    }
    catch (std::exception &e2)
    {
        SUCCEED();
    }
}

TEST_F(L9110_Test_Fixture, VerifyMoveMotor)
{
   /*!
    *  @test Verify L9110_move_motor
    *  is called with the correct parameters
    *  In Sim Mode and Not in Sim Mode
    */

    /* Scenario 1 - Not in Sim Mode */
    EXPECT_CALL(wpimock, pinMode(_, _)).Times(2);
    EXPECT_CALL(wpimock, digitalWrite(_, _)).Times(2);
    L9110 l9110_obj(hw_config);
    
    EXPECT_CALL(wpimock, digitalWrite(hw_config.hw_interface[0].pin_no, 50.00)).Times(1);
    l9110_obj.L9110_move_motor(FOWARD);

    /* Scenario 2 - Not in Sim Mode */
    hw_config.hw_sim_mode = true;
    EXPECT_CALL(wpimock, pinMode(_, _)).Times(0);
    EXPECT_CALL(wpimock, digitalWrite(_, _)).Times(0);
    L9110 l9110_obj1(hw_config);
    
    EXPECT_CALL(wpimock, digitalWrite(_, _)).Times(0);
    l9110_obj1.L9110_move_motor(FOWARD);
}

TEST_F(L9110_Test_Fixture, VerifyMoveMotorRev)
{
   /*!
    *  @test Verify L9110_move_motor
    *  is called with the correct parameters
    */

    /* Scenario 1 - Not in Sim Mode */
    EXPECT_CALL(wpimock, pinMode(_, _)).Times(2);
    EXPECT_CALL(wpimock, digitalWrite(_, _)).Times(2);
    L9110 l9110_obj(hw_config);
    
    EXPECT_CALL(wpimock, digitalWrite(hw_config.hw_interface[1].pin_no, 50.00)).Times(1);
    l9110_obj.L9110_move_motor(REVERSE);
}

TEST_F(L9110_Test_Fixture, VerifyMoveMotorsSTOP)
{
   /*!
    *  @test Verify L9110_move_motor
    *  is called with the correct parameters
    */

    /* Scenario 1 - Not in Sim Mode */
    EXPECT_CALL(wpimock, pinMode(_, _)).Times(2);
    EXPECT_CALL(wpimock, digitalWrite(_, _)).Times(2);
    L9110 l9110_obj(hw_config);
    
    EXPECT_CALL(wpimock, digitalWrite(_, 0.00)).Times(2);
    l9110_obj.L9110_move_motor(STOP);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    MyEnvironment* env = new MyEnvironment(); 
    ::testing::AddGlobalTestEnvironment(env);
    return RUN_ALL_TESTS();
}
