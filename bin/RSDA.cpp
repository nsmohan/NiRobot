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
#include <stdexcept>
#include <thread>
#include <cstring>
#include <string>
#include <getopt.h>
#include <mutex>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_log.h"
#include "RSDA_lib.hpp"
#include "NMT_sock_multi.hpp"
#include "NMT_sock_tcp.hpp"

/*--------------------------------------------------/
/                    Macros                         /
/--------------------------------------------------*/
/** @var MY_NAME
 *  Name of this process */
#define MY_NAME "RSDA"

#define K 1000

/*--------------------------------------------------/
/                Structs/Classes/Enums              /
/--------------------------------------------------*/
struct RSDA_global_data
{
    NMT_sock_multi    *client_sock;
    std::string       multi_cast_ip;
    int               multi_cast_port;
    SensorDataHandler *sensorObj;
    NMT_sock_tcp      *server_sock;
    std::string       tcp_ip;
    int               tcp_port;
    int               rsda_tx_rate;
};

/*--------------------------------------------------/
/                 Prototypes                        /
/--------------------------------------------------*/
static void get_rsda_proc_data(RSXA_procs *procs, int nrof_procs);
static void process_socket_message(std::string message, int client_id);
static void rsda_main_loop();
static void rsda_print_usage(int es);
static void tx_sensor_data_cb();

/*--------------------------------------------------/
/                    Globals                        /
/--------------------------------------------------*/
/** @var SOCK_TIMEOUT
 *  Quantity of Hardware RMCT directly controls*/
const unsigned int SOCK_TIMEOUT = 600;

/** @var global_data
 *  Global Data struct for RSDA */
struct RSDA_global_data global_data = {0};

std::mutex get_data_mtx;

/*--------------------------------------------------------------------/
/                             Start of Program                        /
/--------------------------------------------------------------------*/
using namespace std;
int main(int argc, char *argv[])
{
    /*!
     *  @brief     Main entry point for RSDA
     *  @return    NMT_result
     */

    /** Initialize Varibles */
    int         opt;
    Json::Value SensorData;
    NMT_result  result  = OK;
    RSXA        hw_settings   = {0};
    bool        verbosity     = false;
    bool        continous     = true;

    cout << "Starting Robot Sensor Data Acquisition........" << endl;

    /* 1. Parse Arguments */
    while ((opt = getopt(argc, argv, ":hsv")) != -1)
    {
        switch(opt)
        {
            case 'v':
                cout << "Run in verbose mode ................." << endl;
                verbosity = true;
                break;
             case 's':
                cout << "Single reading Mode ................" << endl;
                continous = false;
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

        /* 4. Get RSDA Settings */
        try
        {
            get_rsda_proc_data(hw_settings.procs, hw_settings.array_len_procs);
            global_data.rsda_tx_rate = K * (hw_settings.general_settings.rsda_broadcast_rate);
        }
        catch(std::exception &e)
        {
            cout << "Error setting up! " << MY_NAME << endl;
            throw;
            exit(1);
        }

        /* 5. Initialize Sensors */
        global_data.sensorObj = new SensorDataHandler(hw_settings, ALL);

        /* 6. Initialize communication sockets */
         global_data.client_sock = new NMT_sock_multi(global_data.multi_cast_port,
                                                      global_data.multi_cast_ip,
                                                      SOCK_CLIENT);

         global_data.server_sock = new NMT_sock_tcp(global_data.tcp_port,
                                                    global_data.tcp_ip,
                                                    SOCK_SERVER);

        /* Start Data Acquisition Timer */
        NMT_stdlib_timer_interrupt(&tx_sensor_data_cb, global_data.rsda_tx_rate, continous);

        /* Run Main Loop */
        if (continous) {rsda_main_loop();}
    }
    else
    {
        cout << "Error getting data from RSXA json file!" << endl;
    }

    NMT_log_finish();
    return result;
}

void get_rsda_proc_data(RSXA_procs *procs, int nrof_procs)
{
    /*!
     *  @brief    Setup Multi-Cast Socket for Program
     *  parm[in]  procs
     *  parm[in]  nrof_procs
     *  @return   void
     */

    /* Initialize Varibles */
    bool found_rsda_elements = false;

    /* Loop over procs until we find RSDA */
    for (int proc_index = 0; proc_index < nrof_procs; proc_index++)
    {
        if (strcmp(procs[proc_index].proc_name, MY_NAME) == 0)
        {
            global_data.multi_cast_port = procs[proc_index].client_p;
            global_data.multi_cast_ip   = procs[proc_index].client_ip;
            global_data.tcp_port        = procs[proc_index].server_p;
            global_data.tcp_ip          = procs[proc_index].server_ip;
            found_rsda_elements = true;
            break;
        }
    }
    if (!found_rsda_elements) {throw std::runtime_error("Error! Unable to find proc data in RSXA Json file");}
}

void process_socket_message(std::string message, int client_id)
{
   
   /* Initialize Varibles */
   Json::Value  action;
   Json::Reader reader;
   
   NMT_log_write(DEBUG, (char *)"Socket event from: %d", client_id);

   /* Parse the message as JSON Object */
   reader.parse(message, action);

   for (Json::Value::ArrayIndex i = 0; i != action.size(); i++)
   {
       if (action[i]["type"].asString() == "proc_action")
       {
           /* Process proc_action */
           if (action[i]["action"].asString() == "exit") 
           
           {
               NMT_log_write(DEBUG, (char *)"Request to terminate %s Recieved", MY_NAME);
               exit(0);
           }
       }
   }
}

void rsda_main_loop()
{ 
    /*!
     *  @brief     RSDA Main Loop
     *  @return    0
     */

    NMT_log_write(DEBUG, (char *)"> ");

    /* Initialize Variables */
    NMT_result result = OK;
    std::string rx_message;
    int client_id;


    /* Listen on socket */
    while (true)
    {
        tie(result, rx_message, client_id) = global_data.server_sock->NMT_read_socket(); 
        if (result == OK)
        {
            thread socket_cb_thread(process_socket_message, rx_message, client_id);
            socket_cb_thread.detach();
        }
    }

    /* Exit the Function */
    NMT_log_write(DEBUG, (char *)"< ");
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

void tx_sensor_data_cb()
{
    /*!
     *  @brief     Call Back for when timer expires
     *  @return    void
     */

    NMT_log_write(DEBUG, (char *)"> ");

    get_data_mtx.lock();

    /* Get Sensor Data */
    Json::Value sensorData = global_data.sensorObj->get_sensor_data();

    /* Transmit the data */
    global_data.client_sock->NMT_write_socket((char *)sensorData.toStyledString().c_str());

    get_data_mtx.unlock();

    /* Exit the Function */
    NMT_log_write(DEBUG, (char *)"< ");
}






