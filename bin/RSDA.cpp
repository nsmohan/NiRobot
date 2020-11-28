/** 
 *  @file      RSDA.cpp
 *  @brief     Robot Sensor Data Acquisition 
 *  @details   Process to gather and transmit robot sensor data 
 *  @author    Nitin Mohan 
 *  @date      November 7, 2020
 *  @copyright 2020
 */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <string>
#include <getopt.h>
#include <jsoncpp/json/json.h>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "RSXA.h"
#include "SensorDataAbstraction.hpp"
#include "NMT_log.h"
#include "NMT_sock_tcp.hpp"

/*--------------------------------------------------/
/                    Macros                         /
/--------------------------------------------------*/
/** @var MY_NAME
 *  Name of this process */
#define MY_NAME "RSDA"

/*--------------------------------------------------/
/                Structs/Classes/Enums              /
/--------------------------------------------------*/
static void rsda_print_usage(int es);

/*--------------------------------------------------------------------/
/                             Start of Program                        /
/--------------------------------------------------------------------*/


class RobotSensorDataAcquisition : public SensorDataAbstraction
{
    public:
        RobotSensorDataAcquisition(RSXA hw_settings, DataMode mode) : 
            SensorDataAbstraction(hw_settings, mode) {};
        ~RobotSensorDataAcquisition() {};
        void get_sensor_data();
};

using namespace std;

void RobotSensorDataAcquisition::get_sensor_data() 
{
    /*!
     *  @brief     Get Robot Sensor Data
     *  @return    N/A
     */


    NMT_log_write(DEBUG, (char *)"> ");

    /* Initialize Variables */
    /* Lopp Over Sonar Sensors and print distance */
    for (auto const &sonar : ultrasonicSensors)
    {
        cout << sonar.first << ": " << sonar.second->distance() << " ";
    }
    cout << endl;

    cout << voltageSensor->ADS115_get_voltage(BATTERY_VOLTAGE) << endl;

    /* Exit the Function */
    NMT_log_write(DEBUG, (char *)"< ");
}


int main(int argc, char *argv[])
{
    /*!
     *  @brief     Main entry point for RSDA
     *  @return    NMT_result
     */

    /** Initialize Varibles */
    int opt;
    NMT_result result  = OK;
    RSXA hw_settings   = {0};
    bool verbosity     = false;

    cout << "Starting Robot Sensor Data Acquisition........" << endl;

    /* 1. Parse Arguments */
    while ((opt = getopt(argc, argv, ":hv")) != -1)
    {
        switch(opt)
        {
            case 'v':
                cout << "Run in verbose mode ................." << endl;
                verbosity = true;
                break;
            case 'h':
                cout << "Help Menu" << endl;
                rsda_print_usage(0);
                break;
            case '?':
                cout << "ERROR, Unrecognized Command!" << endl;
                rsda_print_usage(1);
        }
    }

    /* 2. Get Robot Settings */
    result = RSXA_init(&hw_settings);

    if (result == OK)
    {
        /* 3. Initialize the logger */
        NMT_log_init((char *)hw_settings.log_dir, verbosity);

        RobotSensorDataAcquisition sensorData(hw_settings, ALL);

        sensorData.get_sensor_data();
    }
    return 0;
}

static void rsda_print_usage(int es)
{
    /*!
     *  @brief    Function to print Help Screen
     *  parm[in]  es (Exit Status)
     *  @return   status
     */

    cout << "-v verbosity || -h/help menu" << endl;
    exit(es);
}
