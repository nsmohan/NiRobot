/** 
 *  @file      ads115_test.cpp
 *  @brief     Test Output of ADS115
 *  @details   Get and Print ADS155 Voltage
 *  @author    Nitin Mohan
 *  @date      October 11, 2020
 *  @copyright 2020
 */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <getopt.h>
#include <iostream>
#include <math.h>
#include <unistd.h>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "ADS115.hpp"
#include "RSXA.h"
#include "NMT_log.h"

/*--------------------------------------------------/
/                    Macros                         /
/--------------------------------------------------*/
#define ADS115_str "ADS115"

/*--------------------------------------------------/
/                    Global Varibles                /
/--------------------------------------------------*/
static const std::string BATTERY_VOLTAGE = "BATTERY_VOLTAGE";
static const std::string PI_INPUT_VOLTAGE = "PI_INPUT_VOLTAGE";

/*--------------------------------------------------/
/                Structs/Classes/Enums              /
/--------------------------------------------------*/

/*--------------------------------------------------/
/                   Prototypes                      /
/--------------------------------------------------*/
static void print_usage(int es);
static void get_and_print_voltage(int timeout, ADS115 *ads115_devices);

/*--------------------------------------------------------------------/
/                             Start of Program                        /
/--------------------------------------------------------------------*/
using namespace std;
int main(int argc, char *argv[])
{
    /* Initialize Varibles */
    RSXA hw_settings  = {0};
    NMT_result result = OK;
    bool verbosity    = false;
    bool timeout      = false;
    int opt;
    ADS115 *ads115_devices;

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

        for (int i = 0; i < hw_settings.array_len_hw; i++)
        {
            string hw_name(hw_settings.hw[i].hw_name);

            /* Find ADS115 Settings and create ADS115 object */
            if (hw_name.find(ADS115_str) != string::npos)
            {
                ads115_devices = new ADS115(hw_settings.hw[i]);
                break;
            }
        }

       /* Get and print Voltage */
       get_and_print_voltage(timeout, ads115_devices);

    }
    return 0;
}

static void get_and_print_voltage(int timeout, ADS115 *ads115_devices)
{
    /*!
     *  @brief     Retrive Voltage and Print to Screen
     *  @param[in] ads115_devices
     *  @return    N/A
     */

    NMT_log_write(DEBUG, (char *)"> ");

    /* Initialize Vtribles */
    int delay = pow(10, 6);
    string devices[2] = {BATTERY_VOLTAGE, PI_INPUT_VOLTAGE};

    /* Main Loop */
    while (1)
    {
        for (const string &device : devices)
        {
            cout << device << " = " << ads115_devices->ADS115_get_voltage(device) << endl;
        }
        if(!timeout) {break;}
        usleep(delay);
    }

    /* Exit the Function */
    NMT_log_write(DEBUG, (char *)"< ");
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
