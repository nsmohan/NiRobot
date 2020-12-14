/** 
 *  @file      unittest_RSDA_lib.cc
 *  @brief     Unittests for SensordataAbstraction
 *  @details   Tests for SensorData Abstraction interfaces 
 *  @author    Nitin Mohan
 *  @date      December 06, 2020
 *  @copyright 2020 - NM Technologies
 */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/ 
#include <gtest/gtest.h>
#include <map>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_stdlib.h"
#include "NMT_log.h"
#include "RSDA_lib.hpp"

#define MAX_NROF_HW 3
#define MAX_NROF_PINS 2

/* @class MyEnvironment
 *  Environment Setup for Test */
class MyEnvironment: public ::testing::Environment
{
public:
  virtual ~MyEnvironment() = default;

  virtual void SetUp() {NMT_log_init((char *)"/tmp/", false);}

  virtual void TearDown() {NMT_log_finish();}
};


/* @class RSDA_lib_TestSupport
 * Class to provide access to private methods in RSDA_lib */
class RSDA_lib_TestSupport : public ::testing::Test
{
    public:
        SensorDataHandler *obj;

        void fill_SensorData(const std::string key1, const std::string key2, Json::Value data)
        {
           /*!
            *  @brief     Method to fillSensorData Buffer
            *  @param[in] key1
            *  @param[in] key2
            *  @param[in] data
            *  @return    void
            */

            obj->SensorData["type"] = "TestData";
            obj->SensorData[key1][key2] = data;
            obj->SensorData[key1][key2] = data;
        }

        Json::Value get_sensor_data_var()
        {
           /*!
            *  @brief     Getter to get SensorData
            *  @return    void */

            return obj->SensorData;
        }

        void clear_sensor_data()
        {
           /*!
            *  @brief     Call clear_sensor_data();
            *  @return    void */

            obj->clear_sensor_data();
        }

        void get_and_update_voltage()
        {
           /*!
            *  @brief     Call get_and_update_voltage();
            *  @return    void */

            obj->get_and_update_voltage();
        }

        void get_distance()
        {
           /*!
            *  @brief     Call get_and_update_distance();
            *  @return    void */

            /* Loop over Sonar Sensors and get distance */
            for (auto const &sonar : obj->ultrasonicSensors)
            {
                obj->get_and_update_distance(sonar);;
            }
        }

        void update_sensor_data(const std::string key, const std::string key2, Json::Value data)
        {
           /*!
            *  @brief     Call update_sensor_data();
            *  @return    void */

            obj->update_sensor_data(key, key2, data);
        }
};

/* @class RSDA_Test_Fixture
 * Test Fixture for unittests */
class RSDA_Test_Fixture : public RSDA_lib_TestSupport
{
    public:
        RSXA hw_settings;
        double default_voltage = 6.00;
        double default_distance = 0.17150000000000001;
        std::string HCxSR04_01 = HCxSR04_str + std::string("_01");
        std::string HCxSR04_02 = HCxSR04_str + std::string("_02");

        RSDA_Test_Fixture()
        {
           /*!
            *  @brief     Constructor
            *  @return    void */

            hw_settings.hw = (RSXA_hw*)malloc(sizeof(RSXA_hw) * MAX_NROF_HW);
            hw_settings.array_len_hw = MAX_NROF_HW;
            for (int i = 0; i < MAX_NROF_HW; i++)
            {
                hw_settings.hw[i].hw_interface = (RSXA_pins *)malloc(sizeof(RSXA_pins) * MAX_NROF_PINS);
                strcpy(hw_settings.hw[i].hw_name, (char *)hw[i].c_str());
                hw_settings.hw[i].hw_sim_mode =  true;

                for (int j = 0; j < MAX_NROF_PINS; j++)
                {
                    strcpy(hw_settings.hw[i].hw_interface[j].pin_name, (char *)hw_mapping[hw[i]][j].c_str());
                    hw_settings.hw[i].hw_interface[j].pin_no = j;
                }
            }

            obj = new SensorDataHandler(hw_settings, ALL);
        }

        ~RSDA_Test_Fixture()
        {
           /*!
            *  @brief     Destructor
            *  @return    void */

            for (int i = 0; i < MAX_NROF_HW; i++)
            {
                free(hw_settings.hw[i].hw_interface);
            }
            free(hw_settings.hw);
        }


    private:
        std::string hw[MAX_NROF_HW]           = {ADS115_str, HCxSR04_01, HCxSR04_02};
        std::vector<std::string> ads_pins     = {BATTERY_VOLTAGE, PI_INPUT_VOLTAGE};
        std::vector<std::string> hcxsr04_pins = {echoPin_str, trigPin_str};
        std::map<std::string, std::vector<std::string> > hw_mapping = { {ADS115_str, ads_pins},
                                                                      {HCxSR04_01, hcxsr04_pins},
                                                                      {HCxSR04_02, hcxsr04_pins}};

};

/* ---- Start of Tests -------------*/
using namespace testing;

TEST_F(RSDA_Test_Fixture, VerifyClearSensorDataGW) 
{
   /*!
    *  @test Verify the clear_sensor_data method
    *  @step Inject random data into SensorData buffer
    *  @step Call clear_sensor_data method
    *  @step Call Verfiy buffer is cleared and in clean state
    */

    /* Initialize Variables */
    int maxkeys = 2;
    std::string keys[maxkeys] = {"Proximity", "Voltage"};
    std::string keys2[maxkeys] = {"key1", "key2"};
    int values[maxkeys] = {1, 2};

    for (int i = 0; i < maxkeys; i++)
    {
        /* Fill Data */
        fill_SensorData(keys[i], keys2[i], values[i]);
    }

    /* Act on SUT */
    clear_sensor_data();

    /* Get Buffer value */
    Json::Value SensorData = get_sensor_data_var();

    
    /* Verfiy Results */
    ASSERT_EQ(SensorData["type"].asString(), "SensorData");
    ASSERT_EQ(SensorData["Proximity"].size(), 0);
    ASSERT_EQ(SensorData["Voltage"].size(), 0);
}

TEST_F(RSDA_Test_Fixture, VerifyGetVoltageGW) 
{
   /*!
    *  @test Verfiy Get Voltage Method
    *  @step Call Get Voltage
    *  @step Verfiy result matches expected
    */

    /* Get Updated Voltage */
    get_and_update_voltage();

    /* Get Buffer value */
    Json::Value SensorData = get_sensor_data_var();

    /* Verfiy Results */
    ASSERT_EQ(SensorData["type"].asString(), "SensorData");
    ASSERT_EQ(SensorData["Proximity"].size(), 0);
    ASSERT_DOUBLE_EQ(SensorData["Voltage"][BATTERY_VOLTAGE].asDouble(), default_voltage);
    ASSERT_DOUBLE_EQ(SensorData["Voltage"][PI_INPUT_VOLTAGE].asDouble(), default_voltage);
}

TEST_F(RSDA_Test_Fixture, VerifyGetSonarData) 
{
   /*!
    *  @test Verfiy Get Sonar Data
    *  @step Call Get Distance
    *  @step Verfiy result matches expected
    */

    /* Get Updated Distance */
    get_distance();

    /* Get Buffer value */
    Json::Value SensorData = get_sensor_data_var();

    /* Verfiy Results */
    ASSERT_EQ(SensorData["type"].asString(), "SensorData");
    ASSERT_EQ(SensorData["Voltage"].size(), 0);
    ASSERT_DOUBLE_EQ(SensorData["Proximity"][HCxSR04_01].asDouble(), default_distance);
    ASSERT_DOUBLE_EQ(SensorData["Proximity"][HCxSR04_02].asDouble(), default_distance);
}

TEST_F(RSDA_Test_Fixture, VerifyUpdateSensorData) 
{
   /*!
    *  @test Verfiy Update Sensor Data Method
    *  @step Call Get Update Sensor Data Method w/ different key/value pairs
    *  @step Verfiy result matches expected
    */

    /* Test Data to Inject */
    int maxkeys = 2;
    std::string keys[maxkeys] = {"Proximity", "Voltage"};
    std::string keys2[maxkeys] = {"key1", "key2"};
    int values[maxkeys] = {1, 2};

    /* Update Sensor Data */
    for (int i = 0; i < maxkeys; i++)
    {
        /* Act on SUT */
        update_sensor_data(keys[i], keys2[i], values[i]);
    }

    /* Get Buffer value */
    Json::Value SensorData = get_sensor_data_var();

    /* Verfiy Results */
    ASSERT_EQ(SensorData["type"].asString(), "SensorData");
    for (int i = 0; i < maxkeys; i++)
    {
        ASSERT_DOUBLE_EQ(SensorData[keys[i]][keys2[i]].asDouble(), values[i]);
    }
}

TEST_F(RSDA_Test_Fixture, VerfiyGetSensorData) 
{
   /*!
    *  @test Verify get_sensor_data Method
    *  @step Call Get Sensor Data
    *  @step Verfiy result matches expected
    */

    /* Get Buffer value */
    Json::Value SensorData = obj->get_sensor_data();

    /* Verfiy Results */
    ASSERT_EQ(SensorData["type"].asString(), "SensorData");
    ASSERT_DOUBLE_EQ(SensorData["Proximity"][HCxSR04_01].asDouble(),     default_distance);
    ASSERT_DOUBLE_EQ(SensorData["Proximity"][HCxSR04_02].asDouble(),     default_distance);
    ASSERT_DOUBLE_EQ(SensorData["Voltage"][BATTERY_VOLTAGE].asDouble(),  default_voltage);
    ASSERT_DOUBLE_EQ(SensorData["Voltage"][PI_INPUT_VOLTAGE].asDouble(), default_voltage);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    MyEnvironment* env = new MyEnvironment(); 
    ::testing::AddGlobalTestEnvironment(env);
    return RUN_ALL_TESTS();
}
