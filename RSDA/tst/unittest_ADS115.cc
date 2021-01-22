/** 
 *  @file      unittest_ADS115.cc
 *  @brief     Unittests for ADS115 Library
 *  @details   Unittests for Analog to Digital Converter Library
 *  @author    Nitin Mohan
 *  @date      October 14, 2020
 *  @copyright 2020 - NM Technologies
 */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <gtest/gtest.h>
#include <iostream>
#include <map>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_log.h"
#include "ADS115.hpp"
#include "wiringPi_stub.h"

/*--------------------------------------------------/
/                    Macros                         /
/--------------------------------------------------*/
/**@def ADS115_I2C_ADDRESS
 * ADS115 I2C Address */
#define ADS115_I2C_ADDRESS 0x48

/**@def CONFIG_REG_VALUE
 * Default value for Config Reg*/
#define CONFIG_REG_VALUE 0x0183

/**@def RESET_VALUE
 * Value to trigger Reset */
#define RESET_VALUE 0x06

/**@def MAX_OUTPUT
 * Max Conversion Register Value */
#define MAX_OUTPUT 0x8000

/* @def GAIN_VOLTAGE
 * FS Gain Voltage */
#define GAIN_VOLTAGE 6.144

#define NROF_DEVICES 4

/* @class MyEnvironment
 *  Environment Setup for Test */
class MyEnvironment: public ::testing::Environment
{
public:
  virtual ~MyEnvironment() = default;

  virtual void SetUp() {NMT_log_init((char *)"/tmp/", false);}

  virtual void TearDown() {NMT_log_finish();}
};

/* @class ADS115_Test_Fixture
 * Test Fixture for unittests */
class ADS115_Test_Fixture : public ::testing::Test
{
    public:
        int fd = 1;
        int conversion_reg = 0;
        int config_reg = 1;
        bool sim_mode = false;
        std::string pin_name[NROF_DEVICES] = {"INIT1", "INIT2", "INIT3", "INIT4"};
        RSXA_hw hw_config = {0};
        wiringPiMocker wpimock;
        ~ADS115_Test_Fixture() {};
        
        ADS115_Test_Fixture() 
        {
            /*!
             *  @brief     Constructor
             */

            strcpy(hw_config.hw_name, "ADS115");
            hw_config.hw_sim_mode = sim_mode;
            hw_config.array_len_hw_int = NROF_DEVICES;
            hw_config.hw_interface = (RSXA_pins *)malloc(sizeof(RSXA_pins) * NROF_DEVICES);
            
            for (int i = 0; i < NROF_DEVICES; i++)
            {
                strcpy(hw_config.hw_interface[i].pin_name, pin_name[i].c_str());
                hw_config.hw_interface[i].pin_no = i;
            } 
        };

        void fill_rsxa_pins(RSXA_pins pins)
        {
        }

        ADS115* test_ADS115_creation()
        {
            /*!
             *  @brief     Create ADS115 Object
             *  @return    ADS115 Object
             */

             /* Initialize Variables */
             testing::Sequence seq;

             if (!hw_config.hw_sim_mode)
             {
                 /* Set Expectations */
                 EXPECT_CALL(wpimock, wiringPiI2CSetup(ADS115_I2C_ADDRESS))
                     .Times(1)
                     .WillOnce(testing::Return(fd));
                 EXPECT_CALL(wpimock, wiringPiI2CWriteReg8(fd, conversion_reg, RESET_VALUE))
                     .Times(1)
                     .InSequence(seq);
                 EXPECT_CALL(wpimock, wiringPiI2CWriteReg16(fd, config_reg, NMT_stdlib_swapBytes(CONFIG_REG_VALUE)))
                     .Times(1)
                     .InSequence(seq);
             }

              /* Sensor Object */
              ADS115 *devices = new ADS115(hw_config);
              return devices;
        }

        double get_voltage(ADS115 *devices, std::string deviceName)
        {

            /*!
             *  @brief     Get Voltage from ADC
             *  param[in]  *devices Object
             *  param[in]  deviceName
             *  @return    Voltage 
             */

            /* Initialize Varibles */
            testing::Sequence seq;

            if (!hw_config.hw_sim_mode)
            {
                /* Set Expectations */
                EXPECT_CALL(wpimock, wiringPiI2CReadReg16(fd, config_reg))
                    .Times(1)
                    .InSequence(seq)
                    .WillOnce(testing::Return(CONFIG_REG_VALUE));

                EXPECT_CALL(wpimock, wiringPiI2CWriteReg16(fd, config_reg, testing::_))
                    .Times(1)
                    .InSequence(seq);

                EXPECT_CALL(wpimock, wiringPiI2CReadReg16(fd, config_reg))
                    .Times(1)
                    .InSequence(seq)
                    .WillOnce(testing::Return(0XFFFF));

                EXPECT_CALL(wpimock, wiringPiI2CReadReg16(fd, conversion_reg))
                    .Times(1)
                    .InSequence(seq)
                    .WillOnce(testing::Return(0X2B68));
            }

            /* Act on SUT */
            return devices->ADS115_get_voltage(deviceName);
        }


};

class ADS115_Test_Fixture_BW : public ::testing::Test
{
    public:
        RSXA_hw hw_config = {0};
        ADS115_Test_Fixture_BW() 
        {
            hw_config.array_len_hw_int = 5;
        };
        ~ADS115_Test_Fixture_BW() {};
};

using namespace testing;
/*--------------------------------------------------/
/                   Start of Tests                  /
/--------------------------------------------------*/
TEST_F(ADS115_Test_Fixture, TestADS115CreationSIMFalse)
{
   /*!
    *  @test Object Creation (Sim Mode False)
    *  @step Create ADS115 Objects
    */

    /* Create Sensor Object */
    ADS115 *devices = test_ADS115_creation();
}

TEST_F(ADS115_Test_Fixture, TestADS115CreationSIMTrue)
{
   /*!
    *  @test Object Creation (Sim Mode True)
    *  @step Create ADS115 Objects
    */

    /* Initialize Varibles */
    hw_config.hw_sim_mode = true;

    /* Create Sensor Object */
    ADS115 *devices = test_ADS115_creation();
}

TEST_F(ADS115_Test_Fixture, TestADS115GetVoltageSimFalse)
{
   /*!
    *  @test Get Voltage for All Devices (Sim Mode False)
    *  @step Create ADS115 Objects
    *  @step Get Voltage for all devices
    */

    /* Initialize Varibles */
    double expected_voltage = 5.0000625;

    /* Create Sensor Object */
    ADS115 *devices = test_ADS115_creation();

    for (int i = 0; i < NROF_DEVICES; i++)
    {
        /* Get the Voltage */
        ASSERT_DOUBLE_EQ(get_voltage(devices, pin_name[i]), expected_voltage);
    }
}

TEST_F(ADS115_Test_Fixture, TestADS115GetVoltageSimTrue)
{
   /*!
    *  @test Get Voltage for All Devices (Sim Mode True)
    *  @step Create ADS115 Objects
    *  @step Get Voltage for all devices
    */

    /* Initialize Varibles */
    double expected_voltage = 6.00;
    hw_config.hw_sim_mode = true;

    /* Create Sensor Object */
    ADS115 *devices = test_ADS115_creation();

    for (int i = 0; i < NROF_DEVICES; i++)
    {
        /* Get the Voltage */
        ASSERT_DOUBLE_EQ(get_voltage(devices, pin_name[i]), expected_voltage);
    }
}

TEST_F(ADS115_Test_Fixture_BW, TestADS115CreationSIMFalseBW)
{
   /*!
    *  @test Object Creation Incorrect Number of Devices Passed
    *  @step Create ADS115 Objects
    */

    try
    {
        ADS115 *devices = new ADS115(hw_config);
        FAIL();
    }
    catch (const std::runtime_error& error)
    {
        SUCCEED();
    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    MyEnvironment* env = new MyEnvironment(); 
    ::testing::AddGlobalTestEnvironment(env);
    return RUN_ALL_TESTS();
}

