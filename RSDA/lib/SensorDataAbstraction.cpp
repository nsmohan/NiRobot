/** 
 *  @file      SensorDataAbstraction.cpp
 *  @brief     Sensor Data Abstraction Library
 *  @details   Library between Hardware Drviers and Application Layer
 *  @author    Nitin Mohan 
 *  @date      November 26, 2020 
 *  @copyright 2020
 */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <iostream>
#include <stdexcept>
#include <map>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "SensorDataAbstraction.hpp"
#include "NMT_log.h"

/*--------------------------------------------------/
/                    Macros                         /
/--------------------------------------------------*/

/*--------------------------------------------------/
/                    Globals                        /
/--------------------------------------------------*/

/*--------------------------------------------------------------------/
/                             Start of Program                        /
/--------------------------------------------------------------------*/

using namespace std;
SensorDataAbstraction::SensorDataAbstraction(RSXA hw_settings, DataMode mode) : mode(mode)
{
    /*!
     *  @brief     Constructor
     *  @return    N/A
     */

    /* Loop over hw list */
    for (int i = 0; i < hw_settings.array_len_hw; i++)
    {
        /* Extract elements */
        RSXA_hw hw_element = hw_settings.hw[i];
        string hw_name(hw_element.hw_name);

        /* Check if Sonar HW found */
        if ((hw_name.find(HCxSR04_str) != string::npos) && ((mode == ULTRASONIC) || (mode == ALL)))
        {
            SensorDataAbstraction::init_ultrasonic_sensors(hw_element);
        }
        else if ((hw_name.find(ADS115_str) != string::npos) && ((mode == VOLTAGE) || (mode == ALL)))
        {
            try
            {
                SensorDataAbstraction::init_voltage_detector(hw_element);
            }
            catch (const runtime_error& error)
            {
                throw -1;
            }
        }
    }
}

SensorDataAbstraction::~SensorDataAbstraction()
{
    /*!
     *  @brief     Destructer
     *  @return    N/A
     */

    if ((mode == ALL) || (mode == VOLTAGE))
    {
        delete(voltageSensor);
    }

    if ((mode == ALL) || (mode == ULTRASONIC))
    {
        for (auto const &sonar : ultrasonicSensors)
        {
            delete(sonar.second);
        }
    }

}
void SensorDataAbstraction::init_ultrasonic_sensors(RSXA_hw hw_element)
{
    /*!
     *  @brief     Initialize Ultrasonic Sensors
     *  @param[in] hw_element
     *  @return    N/A
     */

    NMT_log_write(DEBUG, (char *)"> ");

    /* Inner Init */
    bool sim_mode = hw_element.hw_sim_mode;
    int echoPin;
    int trigPin;

    /* Loop over hw interface */
    for (int j = 0; j < hw_element.array_len_hw_int; j++)
    {
        /* Inner Init */
        RSXA_pins interface = hw_element.hw_interface[j];

        /* Extract inerface settings */
        if (string(interface.pin_name) == echoPin_str)
            echoPin = interface.pin_no;
        if (string(interface.pin_name) == trigPin_str)
            trigPin = interface.pin_no;
    }

    /* Create Sonar object */
    ultrasonicSensors.insert(make_pair(hw_element.hw_name, 
                                       new HCxSR04(hw_element.hw_name, trigPin, echoPin, sim_mode)));

    NMT_log_write(DEBUG, (char *)"< ");
}


void SensorDataAbstraction::init_voltage_detector(RSXA_hw hw_element) 
{
    /*!
     *  @brief     Initialize the voltage reader
     *  @param[in] hw_element
     *  @return    N/A
     */

    NMT_log_write(DEBUG, (char *)"> ");

    try
    {
        voltageSensor = new ADS115(hw_element);
    }
    catch (const runtime_error& error)
    {
        NMT_log_write(ERROR, (char *)"Unable to Initialize ADS115 Device");
        throw -1;
    }

    /* Exit the Function */
    NMT_log_write(DEBUG, (char *)"< ");
}

