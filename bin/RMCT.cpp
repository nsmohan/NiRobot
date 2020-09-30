/** 
 *  @file      RMCT.cpp
 *  @brief     Robot Motor Controller
 *  @details   Main Task which allows the control of the hardware motors
 *  @author    Nitin Mohan
 *  @date      March 28, 2020
 *  @copyright 2020 - NM Technologies
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
#include "NMT_log.h"
#include "NMT_sock_tcp.hpp"
#include "RMCT_lib.hpp"

/*--------------------------------------------------/
/                    Macros                         /
/--------------------------------------------------*/
/** @var MY_NAME
 *  Name of this process */
#define MY_NAME "RMCT"

/** @var SOCK_TIMEOUT
 *  Quantity of Hardware RMCT directly controls*/
const unsigned int SOCK_TIMEOUT = 600;

/** @var NO_OF_HW
 *  Quantity of Hardware RMCT directly controls*/
const unsigned int NO_OF_HW = 4;

/*--------------------------------------------------/
/                Structs/Classes/Enums              /
/--------------------------------------------------*/
/** @struct RMCT_hw_settings
 *  Hardware Settings for Robot Motor Controller */
typedef struct RMDR_hw_settings
{
    /** @var pca9685_hw_config
     *  PCA9685 Settings Structure */
    RSXA_hw pca9685_hw_config;

    /** @var left_motor_hw_config
     *  Left Motor Settings Structure*/
    RSXA_hw left_motor_hw_config;

    /** @var right_motor_hw_config
     *  Right Motor Settings Structure*/
    RSXA_hw right_motor_hw_config;

    /** @var cam_motor_hw_config
     *  Camera Motor Settings */
    RSXA_hw cam_motor_hw_config;

    /** @var rmct_hw_settings
     *  RMCT Task Settings */
    RSXA_procs rmct_task_config;

    /** @var camera_motor_sensitivity
     *  Default amount camera should mvoe */
    double camera_motor_sensitivity;
} RMCT_hw_settings;

/*--------------------------------------------------/
/                  Prototypes                       /
/--------------------------------------------------*/
static bool rmct_validate_robot_action(Json::Value mc);
static void rmct_control_print_usage(int es);
static NMT_result rmct_get_robot_settings(RSXA &hw_settings, RMCT_hw_settings &rmct_hw_settings);
static void rmct_main_loop(NMT_sock_tcp server_sock, RobotMotorController rmct_obj);

/*--------------------------------------------------/
/           Entry Point for RMCT Process            /
/--------------------------------------------------*/
using namespace std;
int main(int argc, char *argv[])
{
    /*!
     *  @brief     Main entry point for RMCT
     *  @return    NMT_result
     */

    /** Initialize Varibles */
    int opt;
    RMCT_hw_settings rmct_hw_settings = {0};
    RSXA hw_settings                  = {0};
    NMT_result result                 = OK;
    bool verbosity                    = false;

    cout << "Starting Robot Motor Controller ......" << endl;

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
                rmct_control_print_usage(0);
                break;
            case '?':
                cout << "ERROR, Unrecognized Command!" << endl;
                rmct_control_print_usage(1);
        }
    }

    /* 2. Get Robot Settings */
    result = RSXA_init(&hw_settings);

    /* 3. Validate and Extract Settings Needed */
    if (result == OK)
        result = rmct_get_robot_settings(hw_settings, rmct_hw_settings);

    /* 4. Initialize All Hardware */
    if (result == OK)
    {
        /* Initialize the logger */
        cout << "Initializing the Logger " << hw_settings.log_dir << ".........." << endl;
        NMT_log_init((char *)hw_settings.log_dir, verbosity);

        /* Initialize Robot Motor Controller */
        RobotMotorController rmct_obj(rmct_hw_settings.pca9685_hw_config,
                                      rmct_hw_settings.cam_motor_hw_config,
                                      rmct_hw_settings.left_motor_hw_config,
                                      rmct_hw_settings.right_motor_hw_config,
                                      rmct_hw_settings.camera_motor_sensitivity);

        /** Free RSXA Memory (Everything is initialized) */
        if (result == OK) {RSXA_free_mem(&hw_settings);}

        /* Initialize Sockets */
        RSXA_procs sock_config = rmct_hw_settings.rmct_task_config;
        NMT_sock_tcp server_sock(sock_config.server_p, sock_config.server_ip, SOCK_SERVER, SOCK_TIMEOUT);

        /* 5. Start the Program */
        cout << "RMCT Executed .............. " << endl;
        rmct_main_loop(server_sock, rmct_obj);
    }

    /* Exit the program */
    cout << "Exiting RMCT ........" << endl;
    NMT_log_finish();
    return result;
}

static void rmct_main_loop(NMT_sock_tcp server_sock, RobotMotorController rmct_obj)
{
    /*!
     *  @brief     Main Loop for RMCT
     *  param[in]  server_sock
     *  param[in]  client_sock
     *  param[in]  rmct_obj
     *  @return    NMT_result
     */

    /* Initialize Varibles */
    NMT_result result = OK;
    Json::Value  mc;
    Json::Value  ack;
    Json::Reader reader;
    string rx_message;
    int client_id;
    bool terminate_proc = false;
    ack["type"] = "ack";

    /* Main Program Loop */
    while ((result == OK) && (!terminate_proc))
    {
        /* Listen on the socket */
        tie(result, rx_message, client_id) = server_sock.NMT_read_socket(); 

        if (result == OK)
        {
            /* Parse the message as JSON Object */
            reader.parse(rx_message, mc);

            for (Json::Value::ArrayIndex i = 0; i != mc.size() && result == OK; i++)
            {
                if (rmct_validate_robot_action(mc[i]))
                {
                    if (mc[i]["type"].asString() == "hw_action")
                    {
                        /* Process Request */
                        result = rmct_obj.process_motor_action(mc[i]["motor"].asString(),
                                                               mc[i]["direction"].asString(),
                                                               mc[i]["angle"].asDouble(), 
                                                               mc[i]["speed"].asInt());
                    }
                    else if (mc[i]["type"].asString() == "proc_action")
                    {
                        /* Process proc_action */
                        if (mc[i]["action"].asString() == "exit") {terminate_proc = true;}
                        NMT_log_write(DEBUG, (char *)"Request to terminate RMCT Recieved");
                        result = OK;
                    }
                }
                else
                {
                    result = NOK;
                    NMT_log_write(ERROR, (char *)"Invalid Message Recieved");
                }
            }

            /* Send Acknowledgement */
            ack["result"] = result;
            result = server_sock.NMT_write_socket((char *)(ack.toStyledString()).c_str(), client_id);
        }
    }
    return;
}
static bool rmct_validate_robot_action(Json::Value mc)
{
    /*!
     *  @brief      Verify that the command sent is in the
     *              expected format.
     *  @return     True/False
     */

    NMT_log_write(DEBUG, (char *)">");
    bool valid = false;

    if (mc["type"].isString())
    {
        if (mc["type"].asString() == "hw_action")
        {
            valid = mc["motor"].isString() && mc["direction"].isString() && 
                    mc["angle"].isNumeric() && mc["speed"].isNumeric();
        }
        else if (mc["type"].asString() == "proc_action")
        {
            valid = mc["action"].isString();
        }
    }
        
    /* Check and Exit the Function */
    NMT_log_write (DEBUG, (char *)"< valid=%s", btoa(valid));
    return valid;
}

static NMT_result rmct_get_robot_settings(RSXA &hw_settings, RMCT_hw_settings &rmct_hw_settings)
{
    /*!
     *  @brief      Extract Robot Settings of Interest from RSXA
     *  param[in]   hw_settings (All Hardware Settings)
     *  param[out]  rmct_hw_settings (Extracted Hardware Settings)
     *  @return     NMT_result
     */

    cout << "Validating Robot Settings File........" << endl;
   
    /** Initialize Varibles */
    NMT_result result = OK;
    int mc = 0;

    /** Main Operation */
    if (result == OK)
    {
        /* Camera Motor Sensitivity */
        rmct_hw_settings.camera_motor_sensitivity = hw_settings.general_settings.camera_motor_sensitivity;

        for (int i = 0; i < hw_settings.array_len_hw; i++)
        {
            if (strcmp(hw_settings.hw[i].hw_name, PCA9685_HW_NAME) == 0)
            {
                memcpy(&(rmct_hw_settings.pca9685_hw_config), &(hw_settings.hw[i]), sizeof(hw_settings.hw[i]));
                mc++;
            }
            else if (strcmp(hw_settings.hw[i].hw_name, LEFT_DRV_MTR.c_str()) == 0)
            {
                memcpy(&(rmct_hw_settings.left_motor_hw_config), &(hw_settings.hw[i]), sizeof(hw_settings.hw[i]));
                mc++;
            }
            else if (strcmp(hw_settings.hw[i].hw_name, RIGHT_DRV_MTR.c_str()) == 0)
            {
                memcpy(&(rmct_hw_settings.right_motor_hw_config), &(hw_settings.hw[i]), sizeof(hw_settings.hw[i]));
                mc++;
            }
            else if (strcmp(hw_settings.hw[i].hw_name, LD27MG_HW_NAME) == 0)
            {
                memcpy(&(rmct_hw_settings.cam_motor_hw_config), &(hw_settings.hw[i]), sizeof(hw_settings.hw[i]));
                mc++;
            }

            if (mc == NO_OF_HW) {break;}
        }

        for (int i = 0; i < hw_settings.array_len_procs; i++)
        {
            if (strcmp(hw_settings.procs[i].proc_name, MY_NAME) == 0)
            {
                memcpy(&(rmct_hw_settings.rmct_task_config), &(hw_settings.procs[i]), sizeof(hw_settings.procs[i]));
                mc++;
                break;
            }
        }

        /** Verify we found all the settings needed */
        if (mc != NO_OF_HW + 1)
        {
            cout << "ERROR, Missing Configuration data in RSXA.json file" << endl;
            result = NOK;
        }
    }

    /* Exit the function */
    cout << "Validaton result=" << result_e2s[result] << ".........." << endl;
    return result;
}

static void rmct_control_print_usage(int es)
{
    /*!
     *  @brief    Function to print Help Screen
     *  parm[in]  es (Exit Status)
     *  @return   status
     */

    cout << "-v verbosity || -h/help menu" << endl;
    exit(es);
}
