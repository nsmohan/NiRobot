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
#include <mutex>
#include <stdexcept>
#include <chrono>
#include <thread>
#include <functional>
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

class RSDA_Application : public RobotSensorDataAcquisition
{
    public:
        RSDA_Application(RSXA hw_settings, DataMode mode);
        ~RSDA_Application() {};
        void get_and_transmit_sensor_data();

    private:
        NMT_sock_multi *client_sock;
        int multi_cast_port;
        std::string multi_cast_ip;
        int tcp_port;
        std::string tcp_ip;
        void get_RSDA_parameters(RSXA_procs *procs, int nrof_procs);
};

RSDA_Application::RSDA_Application(RSXA hw_settings, DataMode mode) : RobotSensorDataAcquisition(hw_settings, mode)
{
    try
    {
        get_RSDA_parameters(hw_settings.procs, hw_settings.array_len_procs);

        client_sock = new NMT_sock_multi(multi_cast_port,
                                         multi_cast_ip,
                                         SOCK_CLIENT);
        
    }
    catch (const runtime_error& error)
    {
        throw;
    }
}

void RSDA_Application::get_and_transmit_sensor_data() 
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
        auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(1000);
        client_sock->NMT_write_socket((char *)get_sensor_data().toStyledString().c_str());
        std::this_thread::sleep_until(x);
    }

    /* Exit the Function */
    NMT_log_write(DEBUG, (char *)"< ");
}

void RSDA_Application::get_RSDA_parameters(RSXA_procs *procs, int nrof_procs)
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
        RSDA_Application *rsdaObj = new RSDA_Application(hw_settings, ALL);

        thread t1(&RSDA_Application::get_and_transmit_sensor_data, rsdaObj);
        t1.detach();

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

