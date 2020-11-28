/** 
 *  @file      ultransonic_test.cpp
 *  @brief     Read and display the Ultransonic Sensor Values
 *  @details   Query Ultransonic Sensor Values
 *  @author    Nitin Mohan
 *  @date      August 13, 2020
 *  @copyright 2020 NM Technologies
 */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <getopt.h>
#include <iostream>
#include <map>
#include <unistd.h>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "SensorDataAbstraction.hpp"
#include "RSXA.h"
#include "NMT_log.h"

/*--------------------------------------------------/
/                    Macros                         /
/--------------------------------------------------*/

/*--------------------------------------------------/
/                    Globals                        /
/--------------------------------------------------*/

/*--------------------------------------------------/
/                   Prototypes                      /
/--------------------------------------------------*/
static void print_usage(int es);

/*--------------------------------------------------/
/                   namespace                       /
/--------------------------------------------------*/
using namespace std;

/*--------------------------------------------------------------------/
/                             Start of Program                        /
/--------------------------------------------------------------------*/
class ultrasonic : public SensorDataAbstraction
{
    public:
        ultrasonic(RSXA hw_settings, DataMode mode) : SensorDataAbstraction(hw_settings, mode) {};
        ~ultrasonic() {};
        void output_sensor_data(int timeout);
};

void ultrasonic::output_sensor_data(int timeout)
{

    /*!
     *  @brief     Print Sensor Data to screen
     *  @param[in] timeout
     *  @param[in] sonar_sensors
     *  @return    void
     */
    
    /* Initialize Varibles */
    int delay = 1000000;

    /* Main Loop */
    while (1)
    {
        /* Lopp Over Sonar Sensors and print distance */
        for (auto const &sonar : ultrasonicSensors)
        {
            cout << sonar.first << ": " << sonar.second->distance() << " ";
        }
        cout << endl;
        if(!timeout) {break;}
        usleep(delay);
    }
}

static void print_usage(int es)
{
    /*!
     *  @brief    Function to print Help Screen
     *  parm[in]  es (Exit Status)
     *  @return   status
     */

    cout << "-v verbosity || -h/help menu || -t No Timeout" << endl;
    exit(es);
}

int main(int argc, char *argv[])
{
    /* Initialize Varibles */
    RSXA hw_settings  = {0};
    NMT_result result = OK;
    bool verbosity    = false;
    bool timeout      = false;
    int opt;

    /* 1. Parse Arguments */
    while ((opt = getopt(argc, argv, ":hvt")) != -1)
    {
        switch(opt)
        {
            case 'v':
                cout << "Run in verbose mode ................." << endl;
                verbosity = true;
                break;
            case 't':
                timeout = true;
                break;
            case 'h':
                cout << "Help Menu" << endl;
                print_usage(0);
                break;
            case '?':
                cout << "ERROR, Unrecognized Command!" << endl;
                print_usage(1);
        }
    }

    /* 2. Get RSXA Settings */
    result = RSXA_init(&hw_settings);

    if (result == OK)
    {
        /* 3. Initialize the logger */
        NMT_log_init((char *)hw_settings.log_dir, verbosity);

        /* 4. Initialize Ultrasonic Sensor */
        ultrasonic sensorObj(hw_settings, ULTRASONIC);

        /* Free RSXA Memory - No longer needed */
        RSXA_free_mem(&hw_settings);

        /* 5. Output Sensor Data */
        sensorObj.output_sensor_data(timeout);

        /* Clean Up */
        NMT_log_finish();
     }
    else
    {
        cout << "Error! Failed to get RSXA Settings" << endl;
    }

    /* Exit */
    return 0;
}
