/** 
 *  @file      unittest_HCxSR04.cc
 *  @brief     Unittests for HCxSR04 Library
 *  @details   Unittests Ultrasonic Sensor Library
 *  @author    Nitin Mohan
 *  @date      August 13, 2020
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
#include "NMT_stdlib.h"
#include "NMT_log.h"
#include "HCxSR04.hpp"
#include "wiringPi_stub.h"


/* @class MyEnvironment
 *  Environment Setup for Test */
class MyEnvironment: public ::testing::Environment
{
public:
  virtual ~MyEnvironment() = default;

  virtual void SetUp() {NMT_log_init((char *)"/tmp/", false);}

  virtual void TearDown() {NMT_log_finish();}
};

/* @class HCxSR04_Test_Fixture
 * Test Fixture for unittests */
class HCxSR04_Test_Fixture : public ::testing::Test
{
    public:
        std::string hw_name = "test_sensor";
        int trigger_pin     = 4;
        int echo_pin        = 5;
        bool sim_mode       = true;
        wiringPiMocker wpimock;
        HCxSR04_Test_Fixture() {};
        ~HCxSR04_Test_Fixture() {};

        HCxSR04 test_HCxSR04_creation()
        {
             /* Initialize Variables */
             sim_mode = false;
             testing::Sequence seq;

             /* Set Expectations */
             EXPECT_CALL(wpimock, wiringPiSetup()) .Times(1);
             EXPECT_CALL(wpimock, pinMode(trigger_pin, OUTPUT)).Times(1).InSequence(seq);
             EXPECT_CALL(wpimock, pinMode(echo_pin, INPUT)).Times(1).InSequence(seq);
             EXPECT_CALL(wpimock, digitalWrite(trigger_pin, LOW)).Times(1);

              /* Sensor Object */
              HCxSR04 sensor(hw_name, trigger_pin, echo_pin, sim_mode);
              return sensor;
        }
};

using namespace testing;
/*--------------------------------------------------/
/                   Start of Tests                  /
/--------------------------------------------------*/
TEST_F(HCxSR04_Test_Fixture, TestHCxSR04SimTrueGW)
{
   /*!
    *  @test Get Distance in Simulation Mode compare expected value
    *  @step Set sim_mode = true and verify
    */

    /* Initialize Variables */
    double expected_val = 0.17150;

    /* Sensor Object */
    HCxSR04 sensor(hw_name, trigger_pin, echo_pin, sim_mode);

    /* Get Distance */
    ASSERT_DOUBLE_EQ(expected_val, sensor.distance());
}

TEST_F(HCxSR04_Test_Fixture, TestHCxSR04SimFalseGW)
{
   /*!
    *  @test Object Creation
    *  @step Create Sensor Object and verify inputs
    */

    /* Create Sensor Object */
    HCxSR04 sensor = test_HCxSR04_creation();
}

TEST_F(HCxSR04_Test_Fixture, TestHCxSR04SimFalseGetDistance)
{
   /*!
    *  @test Get Distance when Sim Mode False
    *  @step Create Sensor Object
    *  @step Get the Distance
    */

    /* Create Sensor Object */
    HCxSR04 sensor = test_HCxSR04_creation();

    Sequence seq;

    /* Set Expectations */
    EXPECT_CALL(wpimock, digitalWrite(trigger_pin, HIGH)).Times(1).InSequence(seq);
    EXPECT_CALL(wpimock, digitalWrite(trigger_pin, LOW)).Times(1).InSequence(seq);

    EXPECT_CALL(wpimock, digitalRead(echo_pin)).Times(AtLeast(2))
        .WillOnce(Return(HIGH))
        .WillOnce(Return(LOW))
        .WillOnce(Return(HIGH));

    /* Act On SUT */
    double distance = sensor.distance();
    std::cout << "distance= " << distance << std::endl;
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    MyEnvironment* env = new MyEnvironment(); 
    ::testing::AddGlobalTestEnvironment(env);
    return RUN_ALL_TESTS();
}

