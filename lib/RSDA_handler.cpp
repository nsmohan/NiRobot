/** 
 *  @file      RSDA_handler.cpp
 *  @brief     Sensor Data Abstraction Handler functions 
 *  @details   Functions to get and store Sensor Data 
 *  @author    Nitin Mohan
 *  @date      November 30, 2020 
 *  @copyright 2020
 */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <thread>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "RSDA_handler.hpp"
#include "NMT_log.h"

/*--------------------------------------------------/
/                    Macros                         /
/--------------------------------------------------*/

/*--------------------------------------------------/
/                Structs/Classes/Enums              /
/--------------------------------------------------*/

/*--------------------------------------------------------------------/
/                             Start of Program                        /
/--------------------------------------------------------------------*/
using namespace std;
SensorDataHandler::SensorDataHandler(RSXA hw_settings, DataMode mode) :
            SensorDataAbstraction(hw_settings, mode)
{
    /*!
     *  @brief     Constructor
     *  @param[in] hw_settings
     *  @param[in] mode
     *  @return    N/A
     */

    /* Refresh SensorData Object*/
    clear_sensor_data();
}

/*---------------------------------------------------------------------------------------------------------/
/                                                                                                         /
/                                             Public Methods                                             /
/                                                                                                       /
/-----------------------------------------------------------------------------------------------------*/
Json::Value SensorDataHandler::get_sensor_data() 
{
    /*!
     *  @brief     Get Robot Sensor Data
     *  @return    N/A
     */

    NMT_log_write(DEBUG, (char *)"> ");
    
    /* Initialize Variables */
    vector<thread> distanceSensorThreads;

    /* Reset Var */
    clear_sensor_data();

    /* Loop over Sonar Sensors and get distance */
    for (auto const &sonar : ultrasonicSensors)
    {
        distanceSensorThreads.emplace_back([&](){SensorDataHandler::get_and_update_distance(sonar);});
    }

    /* Get Voltage */
    get_and_update_voltage();

    /* Wait for Sonar Threads to finish */
    for(auto& sonarThread: distanceSensorThreads)
    {
        sonarThread.join();
    }

    /* Exit the Function */
    NMT_log_write(DEBUG, (char *)"< ");
    return SensorData;
}
/*---------------------------------------------------------------------------------------------------------/
/                                                                                                         /
/                                             Private Methods                                            /
/                                                                                                       /
/-----------------------------------------------------------------------------------------------------*/
void SensorDataHandler::clear_sensor_data() 
{
    /*!
     *  @brief     Reset SensorData Object
     *  @return    void
     */


    NMT_log_write(DEBUG, (char *)"> ");

    /* Set Default Values */
    SensorData["type"] = "SensorData";
    SensorData["Proximity"] = Json::arrayValue;
    SensorData["Voltage"] = Json::arrayValue;
    
    /* Clear Arrays to Null */
    SensorData["Proximity"].clear();
    SensorData["Voltage"].clear();


    /* Exit the Function */
    NMT_log_write(DEBUG, (char *)"< ");
}

void SensorDataHandler::get_and_update_distance(std::pair<std::string, HCxSR04*> const &sonar)
{
    /*!
     *  @brief     Get updated distance data from Sonar Sensor
     *  @return    void
     */

    NMT_log_write(DEBUG, (char *)"> ");

    /* Initialize Varibles */
    Json::Value distanceData;
    const string key = "Proximity";

    /* Get new Distance */
    distanceData[sonar.first] = sonar.second->distance();

    /* Update SensorData wtih new value */
    update_sensor_data(key, distanceData);

    /* Exit the function */
    NMT_log_write(DEBUG, (char *)"< ");
}

void SensorDataHandler::get_and_update_voltage() 
{
    /*!
     *  @brief     Get updated voltage data
     *  @return    void
     */

    NMT_log_write(DEBUG, (char *)"> ");

    /* Initialize Variables */
    string voltageDevices[] = {BATTERY_VOLTAGE, PI_INPUT_VOLTAGE};
    Json::Value voltageData;
    const string key = "Voltage";


    /* Get updated voltage for each device */
    for (const string &device : voltageDevices)
    {
        voltageData[device] = voltageSensor->ADS115_get_voltage(device);
    }

    /* Update SensorData with new values */
    update_sensor_data(key, voltageData);

    /* Exit the Function */
    NMT_log_write(DEBUG, (char *)"< ");
}

void SensorDataHandler::update_sensor_data(const std::string key, Json::Value data)
{
    /*!
     *  @brief     Update the Sensor Data varible
     *  @param[in] key
     *  @param[in] data
     *  @return    void
     */


    NMT_log_write(DEBUG, (char *)"> ");

    /* Lock access to Sensor Data */
    DataUpdate_mtx.lock();

    /* Update value */
    SensorData[key].append(data);

    /* Unlock access */
    DataUpdate_mtx.unlock();

    /* Exit the Function */
    NMT_log_write(DEBUG, (char *)"< ");
}
