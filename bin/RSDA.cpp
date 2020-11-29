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
#include <thread>
#include <mutex>
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
#include "NMT_sock_multi.hpp"

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
static NMT_sock_multi* setup_multicast_sockets(RSXA_procs *procs, int nrof_procs);

/*--------------------------------------------------------------------/
/                             Start of Program                        /
/--------------------------------------------------------------------*/

class RobotSensorDataAcquisition : public SensorDataAbstraction
{
    public:
        /* Prototypes */
        RobotSensorDataAcquisition(RSXA hw_settings, DataMode mode);
        ~RobotSensorDataAcquisition() {};
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

using namespace std;

RobotSensorDataAcquisition::RobotSensorDataAcquisition(RSXA hw_settings, DataMode mode) :
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
Json::Value RobotSensorDataAcquisition::get_sensor_data() 
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
        distanceSensorThreads.emplace_back([&](){RobotSensorDataAcquisition::get_and_update_distance(sonar);});
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
void RobotSensorDataAcquisition::clear_sensor_data() 
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

void RobotSensorDataAcquisition::get_and_update_distance(std::pair<std::string, HCxSR04*> const &sonar)
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

void RobotSensorDataAcquisition::get_and_update_voltage() 
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

void RobotSensorDataAcquisition::update_sensor_data(const std::string key, Json::Value data)
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
        RobotSensorDataAcquisition sensorData(hw_settings, ALL);

        /* 5. Join Multi-Cast Group */
        NMT_sock_multi *client_sock = setup_multicast_sockets(hw_settings.procs, hw_settings.array_len_procs);

        SensorData = sensorData.get_sensor_data();
        client_sock->NMT_write_socket((char *)(SensorData.toStyledString()).c_str());
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

static NMT_sock_multi* setup_multicast_sockets(RSXA_procs *procs, int nrof_procs)
{
    /*!
     *  @brief    Setup Multi-Cast Socket for Program
     *  parm[in]  procs
     *  @return   void
     */

    /* Initialize Varibles */
    bool proc_found = false;
    for (int proc_index = 0; proc_index < nrof_procs; proc_index++)
    {
        if (strcmp(procs[proc_index].proc_name, MY_NAME) == 0)
        {
            proc_found = true;
            return new NMT_sock_multi(procs[proc_index].client_p,
                                      procs[proc_index].client_ip,
                                      SOCK_CLIENT,
                                      SOCK_TIMEOUT);
        }
    }

    if (!proc_found)
    {
        cout << "Error! proc_name=" << MY_NAME " Not found in RSXA.json file. Exiting ..." << endl;
        exit(1);
    }
}
