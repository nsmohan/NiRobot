/** 
 *  @file     RSDA_lib.hpp 
 *  @brief    Interfaces to retrive sensor data from abstraction layer 
 *  @details  External interfaces to get sensor data
 *  @author   Nitin Mohan
 *  @date     November 30, 2020
 *  @copyright 2020
 */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <iostream>
#include <jsoncpp/json/json.h>
#include <mutex>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "RSXA.h"
#include "SensorDataAbstraction.hpp"

/*--------------------------------------------------/
/                    Macros                         /
/--------------------------------------------------*/

/*--------------------------------------------------/
/                Structs/Classes/Enums              /
/--------------------------------------------------*/

/*--------------------------------------------------------------------/
/                             Start of Program                        /
/--------------------------------------------------------------------*/
class SensorDataHandler : public SensorDataAbstraction
{
    public:
        /* Prototypes */
        SensorDataHandler(RSXA hw_settings, DataMode mode);
        ~SensorDataHandler() {};
        friend class RSDA_Test_Fixture;
        Json::Value get_sensor_data();

    private:
        /** @var SensorData
         *  Json Structure which is transmitted */
        Json::Value SensorData;

        /** @var DataUpdate_mtx
         *  Mutex to access SensorData */
        std::mutex  DataUpdate_mtx;

        /* Prototypes */
        void clear_sensor_data();
        void get_and_update_distance(std::pair<std::string, HCxSR04*> const &sonar);
        void get_and_update_voltage();
        void update_sensor_data(const std::string key, Json::Value data);
};

