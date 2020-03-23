#include <gtest/gtest.h>
#include "PCA9685.h"
#include "wiringPi_stub.h"
#include "NMT_stdlib.h"
#include "NMT_log.h"
#include <iostream>

#define MODE1 0x00
#define MODE2 0x01
#define PRE_SCALE 0xFE

#define MODE1_INIT 0x21
#define MODE2_INIT 0x04

class MyEnvironment: public ::testing::Environment
{
public:
  virtual ~MyEnvironment() = default;

  virtual void SetUp() {NMT_log_init((char *)"/var/log/NiRobot", false);}

  virtual void TearDown() {NMT_log_finish();}
};

class PCA9685_Test_Fixture : public ::testing::Test
{
    public:
       bool sim_mode;
       PCA9685_settings hw_settings;
       PCA9685_PWM_CHANNEL channel;
       wiringPiMocker wpimock;

       PCA9685_Test_Fixture()
       {
           this->sim_mode = false;
           this->hw_settings = {0};
       }

    protected:

         virtual void SetUp()
         {       
         } 

         virtual void TearDown()
         {
         }
};

using namespace testing;
TEST_F(PCA9685_Test_Fixture, TestPCA9685InitGw)
{
   /*!
    *  @test Call the PCA9685_init Function and
    *  verify correct response. 
    *  @step Set sim_mode = false and verify hardware actions
    *   are performed. 
    *  @step Set sim_mode = true and verify not hardware actions are taken
    */

   /* Set Frequency to 10Hz */
   hw_settings.freq = 30; 

   /* Set fd to 0 */
   hw_settings.fd = 0;

   /* Expected pre_scale for 10Hz */
   int pre_scale = 202;

   /* Not in Sim Mode */
   this->sim_mode = false;

   /* Verify Correct values are passed in to HW action functions */
   EXPECT_CALL(wpimock, wiringPiI2CWriteReg8(0, 
               AnyOf(MODE1, MODE2, PRE_SCALE), 
               AnyOf(MODE1_INIT, MODE2_INIT, pre_scale)))
       .Times(3);
   ASSERT_EQ(OK, PCA9685_init(&hw_settings, sim_mode));

   /* In Sim Mode */
   sim_mode = true;
   EXPECT_CALL(wpimock, wiringPiI2CWriteReg8(_, _, _)).Times(0);
   ASSERT_EQ(OK, PCA9685_init(&hw_settings, sim_mode));

}

TEST_F(PCA9685_Test_Fixture, TestSetFreqMaxMin)
{
   /*!
    *  @test Verify the PCA9685_setFreq caps the max/min
    *        freqeucy apprppriatly. 
    *  @step Set sim_mode = false and verify hardware actions
    *   are performed. 
    *  @step Set sim_mode = true and verify not hardware actions are taken
    */

   /* Set fd to 0 */
   hw_settings.fd = 0;


   /* Not in Sim Mode */
   sim_mode = false;

   /* Verify Correct values are passed in to HW action functions */

   /* Set Frequency to 10Hz */
   hw_settings.freq = 10; /* We Expect Min to be 30 */

   /* Expected pre_scale for 10Hz */
   int pre_scale = 202;

   EXPECT_CALL(wpimock, wiringPiI2CWriteReg8(0, 
               AnyOf(MODE1, MODE2, PRE_SCALE), 
               AnyOf(MODE1_INIT, MODE2_INIT, pre_scale)))
       .Times(3);
   ASSERT_EQ(OK, PCA9685_init(&hw_settings, sim_mode));

   /* Set Frequency to 10Hz */
   hw_settings.freq = 1510; /* We Expect Max to be 1510 */

   pre_scale = 3;

   EXPECT_CALL(wpimock, wiringPiI2CWriteReg8(0, 
               AnyOf(MODE1, MODE2, PRE_SCALE), 
               AnyOf(MODE1_INIT, MODE2_INIT, pre_scale)))
       .Times(3);
   ASSERT_EQ(OK, PCA9685_init(&hw_settings, sim_mode));
}

TEST_F(PCA9685_Test_Fixture, TestPCA9685InitBw)
{
   /*!
    *  @test Call the PCA9685_init Function and
    *  verify correct response. (Result = NOK)
    *  @step Set sim_mode = false and verify hardware actions
    *   are performed. 
    *  @step Set sim_mode = true and verify not hardware actions are taken
    */

   /* Set fd to 0 */
   hw_settings.fd = -1;

   /* Not in Sim Mode */
   sim_mode = false;

   /* Verify Correct values are passed in to HW action functions */
   EXPECT_CALL(wpimock, wiringPiI2CWriteReg8(_ ,_ , _)).Times(0);
   ASSERT_EQ(NOK, PCA9685_init(&hw_settings, sim_mode));

   /* In Sim Mode */
   sim_mode = true;
   EXPECT_CALL(wpimock, wiringPiI2CWriteReg8(_, _, _)).Times(0);
   ASSERT_EQ(NOK, PCA9685_init(&hw_settings, sim_mode));
}

TEST_F(PCA9685_Test_Fixture, TestchgFreqGW)
{
   /*!
    *  @test Call the PCA9685_chgFreq Function and
    *  verify correct response. (Result = OK)
    *  @step Set sim_mode = false and verify hardware actions
    *   are performed. 
    *  @step Set sim_mode = true and verify not hardware actions are taken
    */

    int orig_value = 0x21;
    int slp_value = 0x31;
    int pre_scale = 151;

    /* Set Variable values */
    hw_settings.fd = 0;
    hw_settings.freq = 40;
    

    /* Set Simulation Mode to false and set Expectations */
    sim_mode = false;
    EXPECT_CALL(wpimock, wiringPiI2CReadReg8(hw_settings.fd, MODE1))
            .Times(1)
            .WillOnce(Return(orig_value));
    EXPECT_CALL(wpimock, wiringPiI2CWriteReg8(hw_settings.fd, _, 
            AnyOf(orig_value, slp_value, pre_scale)))
            .Times(3);
    ASSERT_EQ(OK, PCA9685_chgFreq(&hw_settings, sim_mode));

    /* Set Simulation Mode to false and set Expectations */
    sim_mode = true;
    EXPECT_CALL(wpimock, wiringPiI2CReadReg8(_, _))
           .Times(0);
    EXPECT_CALL(wpimock, wiringPiI2CWriteReg8(_, _, _))
            .Times(0);
    ASSERT_EQ(OK, PCA9685_chgFreq(&hw_settings, sim_mode));
}

TEST_F(PCA9685_Test_Fixture, TestchgFreqBW)
{
   /*!
    *  @test Call the PCA9685_chgFreq Function and
    *  verify correct response. (Result = NOK)
    *  @step Set sim_mode = false and verify hardware actions
    *   are performed. 
    *  @step Set sim_mode = true and verify not hardware actions are taken
    */

    /* Set Variable values */
    hw_settings.fd = -1;
    hw_settings.freq = 40;
    

    /* Set Simulation Mode to false and set Expectations */
    sim_mode = false;
    EXPECT_CALL(wpimock, wiringPiI2CReadReg8(_, _))
           .Times(0);
    EXPECT_CALL(wpimock, wiringPiI2CWriteReg8(_, _, _))
            .Times(0);
    ASSERT_EQ(NOK, PCA9685_chgFreq(&hw_settings, sim_mode));

    /* Set Simulation Mode to false and set Expectations */
    sim_mode = true;
    EXPECT_CALL(wpimock, wiringPiI2CReadReg8(_, _))
           .Times(0);
    EXPECT_CALL(wpimock, wiringPiI2CWriteReg8(_, _, _))
            .Times(0);
    ASSERT_EQ(NOK, PCA9685_chgFreq(&hw_settings, sim_mode));
}

TEST_F(PCA9685_Test_Fixture, TestsetPWMGW)
{
   /*!
    *  @test Call PCA9685_setPWM and
    *  verify the correct values are written to registers
    *  @step Set sim_mode = false and verify hardware actions
    *   are performed. 
    *  @step Set sim_mode = true and verify not hardware actions are taken
    */

    /* Set Variable values */
    hw_settings.fd = 0;
    hw_settings.duty_cycle = 50;
    hw_settings.delay_time = 100;
    channel = CHANNEL_0;
    int ch1 = channel * 4 + 0x06;
    int ch2 = ch1 + 2;
    int tics_to_on = 4095;
    int tics_to_off = 6143;
    

    /* Set Simulation Mode to false and set Expectations */
    sim_mode = false;
    EXPECT_CALL(wpimock, wiringPiI2CWriteReg16(hw_settings.fd, 
                AnyOf(ch1, ch2),  AnyOf(tics_to_on, tics_to_off)))
            .Times(2);
    ASSERT_EQ(OK, PCA9685_setPWM(&hw_settings, 
                                     channel, sim_mode));

    /* Set Simulation Mode to false and set Expectations */
    sim_mode = true;
    EXPECT_CALL(wpimock, wiringPiI2CWriteReg16(hw_settings.fd, _, _))
            .Times(0);
    ASSERT_EQ(OK, PCA9685_setPWM(&hw_settings, channel, sim_mode));
}

TEST_F(PCA9685_Test_Fixture, TestsetPWMBW)
{
   /*!
    *  @test Call PCA9685_setPWM and
    *  verify result = NOK if no hardware data pointer is passed
    *  @step Set sim_mode = false and verify hardware actions
    *   are performed. 
    *  @step Set sim_mode = true and verify not hardware actions are taken
    */

    /* Set Variable values */
    hw_settings.fd = -1;
    

    /* Set Simulation Mode to false and set Expectations */
    sim_mode = false;
    EXPECT_CALL(wpimock, wiringPiI2CWriteReg16(hw_settings.fd, _, _))
            .Times(0);
    ASSERT_EQ(NOK, PCA9685_setPWM(&hw_settings, 
                                     channel, sim_mode));

    /* Set Simulation Mode to false and set Expectations */
    sim_mode = true;
    EXPECT_CALL(wpimock, wiringPiI2CWriteReg16(hw_settings.fd, _, _))
            .Times(0);
    ASSERT_EQ(NOK, PCA9685_setPWM(&hw_settings, channel, sim_mode));
}

TEST_F(PCA9685_Test_Fixture, TestsetPWM_MaxMin)
{
   /*!
    *  @test Call PCA9685_setPWM and
    *  verify duty_cycle Max Min values are trimmed
    *  @step Set sim_mode = false and verify hardware actions
    *   are performed. 
    *  @step Set sim_mode = true and verify not hardware actions are taken
    */

    /* Set Variable values */
    hw_settings.fd = 0;
    hw_settings.delay_time = 100;
    channel = CHANNEL_5;
    int ch1 = channel * 4 + 0x06;
    int ch2 = ch1 + 2;
    int tics_to_on = 4095;
    int tics_to_off = 8191;

    int duty_cycles_cases[] = {-100, 200};
    

    /* Set Expectations and call the function */
    for (int i = 0; i < 2; i++)
    {
        hw_settings.duty_cycle = duty_cycles_cases[i];
        EXPECT_CALL(wpimock, wiringPiI2CWriteReg16(hw_settings.fd, 
                    AnyOf(ch1, ch2),  AnyOf(tics_to_on, tics_to_off)))
                .Times(2);
        ASSERT_EQ(OK, PCA9685_setPWM(&hw_settings, 
                                         channel, sim_mode));
    }

}

TEST_F(PCA9685_Test_Fixture, TestPCA9685GetPWMGW)
{
   /*!
    *  @test Call PCA9685_getPWM and
    *  verify duty_cycle is read from HW and returned
    *  @step Set sim_mode = false and verify hardware actions
    *   are performed. 
    *  @step Set sim_mode = true and verify not hardware actions are taken
    */

    /* Set Variable values */
    hw_settings.fd = 0;
    channel = CHANNEL_5;
    int ch1 = channel * 4 + 0x06;
    int ch2 = ch1 + 2;
    int tics_to_on = 4095;
    int tics_to_off = 8191;
    double exp_duty;
    double precison = 0.0001;

    

    /* Set Expectations and call the function */
    sim_mode = false;
    exp_duty = 99.9878;
    EXPECT_CALL(wpimock, wiringPiI2CReadReg16(hw_settings.fd, 
                AnyOf(ch1, ch2)))
            .Times(2)
            .WillOnce(Return(tics_to_on))
            .WillOnce(Return(tics_to_off));
    ASSERT_EQ(OK, PCA9685_getPWM(&hw_settings, 
                                     channel, sim_mode));
    EXPECT_NEAR(exp_duty, hw_settings.duty_cycle, precison);

    /* Test the function is not called in sim_mode */
    sim_mode = true;
    exp_duty = 12.1948;
    EXPECT_CALL(wpimock, wiringPiI2CReadReg16(_, _))
            .Times(0);
    ASSERT_EQ(OK, PCA9685_getPWM(&hw_settings, 
                                     channel, sim_mode));
    EXPECT_NEAR(exp_duty, hw_settings.duty_cycle, precison);
}

TEST_F(PCA9685_Test_Fixture, TestgetPWMBW)
{
   /*!
    *  @test Call PCA9685_getPWM and
    *  verify result = NOK if no hardware data pointer is passed
    *  @step Set sim_mode = false and verify hardware actions
    *   are performed. 
    *  @step Set sim_mode = true and verify not hardware actions are taken
    */

    /* Set Variable values */
    hw_settings.fd = -1;
    

    /* Set Simulation Mode to false and set Expectations */
    sim_mode = false;
    EXPECT_CALL(wpimock, wiringPiI2CReadReg16(hw_settings.fd, _))
            .Times(0);
    ASSERT_EQ(NOK, PCA9685_getPWM(&hw_settings, 
                                     channel, sim_mode));

    /* Set Simulation Mode to false and set Expectations */
    sim_mode = true;
    EXPECT_CALL(wpimock, wiringPiI2CReadReg16(hw_settings.fd, _))
            .Times(0);
    ASSERT_EQ(NOK, PCA9685_getPWM(&hw_settings, channel, sim_mode));
}

TEST_F(PCA9685_Test_Fixture, TestPCA9685GetStatusGW)
{
   /*!
    *  @test Call PCA9685_get_init_status
    *  verify hardware calls are made correctly
    *  @step Set sim_mode = false and verify hardware actions
    *   are performed. 
    *  @step Set sim_mode = true and verify not hardware actions are taken
    */

    /* Set Variable values */
    hw_settings.fd = 0;
    hw_settings.freq = 30.00;
    int pre_scale = 202;

    /* T1 Expect system to be initialized */
    sim_mode = false;
    bool initialized = false;
    EXPECT_CALL(wpimock, wiringPiI2CReadReg8(hw_settings.fd, 
             AnyOf(MODE1, MODE2, PRE_SCALE)))
            .Times(3)
            .WillOnce(Return(MODE1_INIT))
            .WillOnce(Return(MODE2_INIT))
            .WillOnce(Return(pre_scale));
    ASSERT_EQ(OK, PCA9685_get_init_status(&hw_settings,
                                              &initialized, 
                                              sim_mode));
    ASSERT_EQ(true, initialized);


    /* Test the function is not called in sim_mode */
    sim_mode = true;
    EXPECT_CALL(wpimock, wiringPiI2CReadReg8(_, _))
            .Times(0);
}

TEST_F(PCA9685_Test_Fixture, TestPCA9685GetStatusFalseGW)
{
   /*!
    *  @test Call PCA9685_get_init_status
    *  verify init flag is not set if one of the ret status
    *  does not match
    *  @step Set sim_mode = false and verify hardware actions
    *   are performed. 
    *  @step Set sim_mode = true and verify not hardware actions are taken
    */

    /* Set Variable values */
    bool initialized;
    hw_settings.fd = 0;
    hw_settings.freq = 30.00;
    int pre_scale = 202;
    int gvalue = 100;

    /* MODE1_INIT does not match */
    initialized = false;
    EXPECT_CALL(wpimock, wiringPiI2CReadReg8(hw_settings.fd, 
             AnyOf(MODE1, MODE2, PRE_SCALE)))
            .Times(3)
            .WillOnce(Return(gvalue))
            .WillOnce(Return(MODE2_INIT))
            .WillOnce(Return(pre_scale));
    ASSERT_EQ(OK, PCA9685_get_init_status(&hw_settings,
                                              &initialized, 
                                              sim_mode));
    ASSERT_EQ(false, initialized);

    /* MODE2_INIT does not match */
    initialized = false;
    EXPECT_CALL(wpimock, wiringPiI2CReadReg8(hw_settings.fd, 
             AnyOf(MODE1, MODE2, PRE_SCALE)))
            .Times(3)
            .WillOnce(Return(MODE1_INIT))
            .WillOnce(Return(gvalue))
            .WillOnce(Return(pre_scale));
    ASSERT_EQ(OK, PCA9685_get_init_status(&hw_settings,
                                              &initialized, 
                                              sim_mode));
    ASSERT_EQ(false, initialized);

    /* Frequency does not match */
    initialized = false;
    EXPECT_CALL(wpimock, wiringPiI2CReadReg8(hw_settings.fd, 
             AnyOf(MODE1, MODE2, PRE_SCALE)))
            .Times(3)
            .WillOnce(Return(MODE1_INIT))
            .WillOnce(Return(MODE2_INIT))
            .WillOnce(Return(gvalue));
    ASSERT_EQ(OK, PCA9685_get_init_status(&hw_settings,
                                              &initialized, 
                                              sim_mode));
    ASSERT_EQ(false, initialized);
}

TEST_F(PCA9685_Test_Fixture, TestPCA9685GetStatusBW)

{
   /*!
    *  @test Call PCA9685_get_init_status
    *  verify return is NOK if bad fd is passed
    *  does not match
    *  @step Set sim_mode = false and verify hardware actions
    *   are performed. 
    *  @step Set sim_mode = true and verify not hardware actions are taken
    */

    /* Set Variable values */
    bool initialized;
    hw_settings.fd = -1; /* Invalid file pointer */

    /* MODE1_INIT does not match */
    initialized = false;
    EXPECT_CALL(wpimock, wiringPiI2CReadReg8(_, _))
       .Times(0); 
    ASSERT_EQ(NOK, PCA9685_get_init_status(&hw_settings,
                                              &initialized, 
                                              sim_mode));
    ASSERT_EQ(false, initialized);
    /* Set Variable values */
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    MyEnvironment* env = new MyEnvironment(); 
    ::testing::AddGlobalTestEnvironment(env);
    return RUN_ALL_TESTS();
}
