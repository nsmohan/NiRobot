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
#include "HCxSR04.hpp"
#include "RSXA.h"
#include "NMT_stdlib.h"
#include "NMT_log.h"

/*--------------------------------------------------/
/                    Macros                         /
/--------------------------------------------------*/
#define HCxSR04_str "HCxSR04"
#define echoPin_str "echoPin"
#define trigPin_str "trigPin"

/*--------------------------------------------------/
/                    Globals                        /
/--------------------------------------------------*/

/*--------------------------------------------------/
/                   Prototypes                      /
/--------------------------------------------------*/
static std::map<std::string, HCxSR04*> get_sonar_sensors(RSXA hw_settings);
static void output_sensor_data(int timeout, std::map<std::string, HCxSR04*> sonar_sensors);
static void clean_up(std::map<std::string, HCxSR04*> sonar_sensors);
static void print_usage(int es);

/*--------------------------------------------------/
/                   namespace                       /
/--------------------------------------------------*/
using namespace std;

/*--------------------------------------------------------------------/
/                             Start of Program                        /
/--------------------------------------------------------------------*/
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

        /* 4. Get Sensor Objects */
        map<string, HCxSR04*> sonar_sensors = get_sonar_sensors(hw_settings);

        /* Free RSXA Memory - No longer needed */
        RSXA_free_mem(&hw_settings);

        /* 5. Output Sensor Data */
        output_sensor_data(timeout, sonar_sensors);

        /* Clean Up */
        clean_up(sonar_sensors);
     }
    else
    {
        cout << "Error! Failed to get RSXA Settings" << endl;
    }

    /* Exit */
    return 0;
}

void output_sensor_data(int timeout, map<string, HCxSR04*> sonar_sensors)
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
        for (auto const &sonar : sonar_sensors)
        {
            cout << sonar.first << ": " << sonar.second->distance() << " ";
        }
        cout << endl;
        if(!timeout) {break;}
        usleep(delay);
    }
}

static void clean_up(map<string, HCxSR04*> sonar_sensors)
{
    /*!
     *  @brief     Clean Up All used memory
     *  @param[in] sonar_sensors
     *  @return    void
     */

    /* Lopp Over Sonar Sensors and delete object */
    for (auto const &sonar : sonar_sensors)
    {
        delete(sonar.second);
    }

    /* Close Logger */
    NMT_log_finish();
}

map<string, HCxSR04*> get_sonar_sensors(RSXA hw_settings)
{
    /*!
     *  @brief     Create HCxSR04 Objects based on 
     *             RSXA settings
     *  @param[in] hw_settings
     *  @return    sensors 
     */

    /* Initialize Varibles */
    std::map<std::string, HCxSR04*> sensors;

    /* Loop over hw list */
    for (int i = 0; i < hw_settings.array_len_hw; i++)
    {
        /* Extract elements */
        RSXA_hw hw_element = hw_settings.hw[i];
        string hw_name(hw_element.hw_name);

        /* Check if Sonar HW found */
        if (hw_name.find(HCxSR04_str) != string::npos)
        {
            /* Inner Init */
            bool sim_mode = hw_element.hw_sim_mode;
            int echoPin;
            int trigPin;

            /* Loop over hw interface */
            for (int j = 0; j < hw_settings.hw[i].array_len_hw_int; j++)
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
            sensors.insert(make_pair(hw_name, new HCxSR04(hw_name, trigPin, echoPin, sim_mode)));
        }
    }

    /* Exit the function */
    return sensors;
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
