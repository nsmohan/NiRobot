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
#include <chrono>
#include <thread>
#include <cstring>
#include <string>
#include <getopt.h>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "RSDA.hpp"
#include "NMT_log.h"

/*--------------------------------------------------/
/                    Macros                         /
/--------------------------------------------------*/
/** @var MY_NAME
 *  Name of this process */
#define MY_NAME "RSDA"

/*--------------------------------------------------/
/                    Globals                        /
/--------------------------------------------------*/
/** @var SOCK_TIMEOUT
 *  Quantity of Hardware RMCT directly controls*/
const unsigned int SOCK_TIMEOUT = 600;

/*--------------------------------------------------/
/                Structs/Classes/Enums              /
/--------------------------------------------------*/
static void rsda_print_usage(int es);

/*--------------------------------------------------------------------/
/                             Start of Program                        /
/--------------------------------------------------------------------*/
using namespace std;
RSDA::RSDA(RSXA hw_settings, DataMode mode) : SensorDataHandler(hw_settings, mode)
{
    try
    {
        get_RSDA_parameters(hw_settings.procs, hw_settings.array_len_procs);

        client_sock = new NMT_sock_multi(multi_cast_port,
                                         multi_cast_ip,
                                         SOCK_CLIENT);

        server_sock = new NMT_sock_tcp(tcp_port,
                                       tcp_ip,
                                       SOCK_SERVER);
        
    }
    catch (const runtime_error& error)
    {
        throw;
    }
}

void RSDA::get_and_transmit_sensor_data(int interval) 
{
    /*!
     *  @brief     Description
     *  @param[in]
     *  @param[out]
     *  @return    0
     */

    NMT_log_write(DEBUG, (char *)"> ");

    while (true)
    {
        auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(interval);
        client_sock->NMT_write_socket((char *)get_sensor_data().toStyledString().c_str());
        std::this_thread::sleep_until(x);
    }

    /* Exit the Function */
    NMT_log_write(DEBUG, (char *)"< ");
}


void RSDA::server_listener()
{
    /*!
     *  @brief     Description
     *  @param[in]
     *  @param[out]
     *  @return    0
     */


    NMT_log_write(DEBUG, (char *)"> ");

    /* Initialize Variables */
    NMT_result result = OK;
    string rx_message;
    int client_id;
    Json::Value  action;
    Json::Reader reader;


    tie(result, rx_message, client_id) = server_sock->NMT_read_socket(); 
    /* Parse the message as JSON Object */
    reader.parse(rx_message, action);

    for (Json::Value::ArrayIndex i = 0; i != action.size() && result == OK; i++)
    {
        if (action[i]["type"].asString() == "proc_action")
        {
            /* Process proc_action */
            if (action[i]["action"].asString() == "exit") 
            
            {
                
                NMT_log_write(DEBUG, (char *)"Request to terminate RMCT Recieved");
                exit(0);
            }
        }
    }

    /* Exit the Function */
    NMT_log_write(DEBUG, (char *)"< ");
}

void RSDA::get_RSDA_parameters(RSXA_procs *procs, int nrof_procs)
{
    /*!
     *  @brief    Setup Multi-Cast Socket for Program
     *  parm[in]  procs
     *  @return   void
     */

    /* Initialize Varibles */
    bool found_rsda_elements = false;

    for (int proc_index = 0; proc_index < nrof_procs; proc_index++)
    {
        if (strcmp(procs[proc_index].proc_name, MY_NAME) == 0)
        {
            multi_cast_port = procs[proc_index].client_p;
            multi_cast_ip = procs[proc_index].client_ip;
            tcp_port = procs[proc_index].server_p;
            tcp_ip = procs[proc_index].server_ip;
            
            found_rsda_elements = true;
            break;
        }
    }
    if (!found_rsda_elements) {throw std::runtime_error("Error! Unable to find proc data in RSXA Json file");}
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
    Json::Value SensorData;

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

        /* 4. Initialize Sensors */
        RSDA *rsdaObj = new RSDA(hw_settings, ALL);

        thread t1(&RSDA::get_and_transmit_sensor_data, rsdaObj, 1000);
        thread t2(&RSDA::server_listener, rsdaObj);
        t1.join();
        t2.join();

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

