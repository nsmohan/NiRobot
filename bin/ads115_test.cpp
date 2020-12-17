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
#include <map>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "RSXA.h"
#include "SensorDataAbstraction.hpp"
#include "NMT_log.h"

/*--------------------------------------------------/
/                    Macros                         /
/--------------------------------------------------*/

/*--------------------------------------------------/
/                    Global Varibles                /
/--------------------------------------------------*/
static const int NROF_DEVICES = 2;

/*--------------------------------------------------/
/                   Prototypes                      /
/--------------------------------------------------*/
static void print_usage(int es);

/*--------------------------------------------------------------------/
/                             Start of Program                        /
/--------------------------------------------------------------------*/
class ads115_test : public SensorDataAbstraction
{
    public:
        ads115_test(RSXA hw_settings, DataMode mode) : SensorDataAbstraction(hw_settings, mode) {};
        ~ads115_test() {};
        void get_and_print_voltage(int timeout);
};


using namespace std;
void ads115_test::get_and_print_voltage(int timeout)
{
    /*!
     *  @brief     Retrive Voltage and Print to Screen
     *  @param[in] ads115_devices
     *  @return    N/A
     */

    NMT_log_write(DEBUG, (char *)"> ");

    /* Initialize Vtribles */
    int delay = pow(10, 6);
    string devices[NROF_DEVICES] = {BATTERY_VOLTAGE, PI_INPUT_VOLTAGE};

    /* Main Loop */
    while (1)
    {
        for (const string &device : devices)
        {
            cout << device 
                 << " = " 
                 << voltageSensor->ADS115_get_voltage(device) 
                 << "v" 
                 << " ";
        }
        cout << endl;
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

    /* 1. Get RSXA Settings */
    result = RSXA_init(&hw_settings);

    if (result == OK)
    {
        /* 2. Initialize the logger */
        NMT_log_init((char *)hw_settings.log_dir, verbosity);

        try
        {
            /* 3. Get ADS Devices */
            ads115_test voltReader(hw_settings, VOLTAGE);

            /* 4. Get and print Voltage */
            voltReader.get_and_print_voltage(timeout);
        }
        catch (int e)
        {
            return -1;
        }
    }

    /* Clean up */
    NMT_log_finish();
    RSXA_free_mem(&hw_settings);

    /* Exit */
    return 0;
}


