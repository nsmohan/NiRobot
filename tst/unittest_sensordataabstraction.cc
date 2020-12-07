/** 
 *  @file      unittest_sensordataabstraction.cc
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

#define MAX_NROF_HW 2
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

/* @class RSDA_Test_Fixture
 * Test Fixture for unittests */
class RSDA_Test_Fixture : public ::testing::Test
{
    public:
        RSXA hw_settings;
        SensorDataHandler *obj;


        RSDA_Test_Fixture()
        {
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
            for (int i = 0; i < MAX_NROF_HW; i++)
            {
                free(hw_settings.hw[i].hw_interface);
                free(hw_settings.hw);
            }
        }

    private:
        std::string hw[MAX_NROF_HW]           = {ADS115_str, HCxSR04_str};
        std::vector<std::string> ads_pins     = {BATTERY_VOLTAGE, PI_INPUT_VOLTAGE};
        std::vector<std::string> hcxsr04_pins = {echoPin_str, trigPin_str};
        std::map<std::string, std::vector<std::string> > hw_mapping = { {ADS115_str, ads_pins},
                                                                      {HCxSR04_str, hcxsr04_pins}};

};

/* ---- Start of Tests -------------*/
using namespace testing;

TEST_F(RSDA_Test_Fixture, VerifyClearSensorDataGW) 
{

    //handler_obj.SensorData["Testing"] = "Test";
    
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    MyEnvironment* env = new MyEnvironment(); 
    ::testing::AddGlobalTestEnvironment(env);
    return RUN_ALL_TESTS();
}
