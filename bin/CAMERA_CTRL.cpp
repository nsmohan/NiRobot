/* CAM_CTRL.cpp: External Interface to allow for camera control. 
 *               User is able to move the motor (UP, DOWN,LEFT,RIGHT,HOME)
 *               HOME = 90/90

__author__      = "Nitin Mohan
__copyright__   = "Copy Right 2019. NM Technologies" */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <iostream>
#include <string>
#include <algorithm>
#include <getopt.h>
#include <unistd.h>
/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "CAM_MOTOR_CTRL.hpp"
#include "NMT_stdlib.h"
#include "NMT_log.h"
#include "MTDR.h"

/*--------------------------------------------------/
/                   Macros                          /
/--------------------------------------------------*/
#define LOG_DIR "/var/log/NiRobot"

/*--------------------------------------------------/
/                   Global Vars                     /
/--------------------------------------------------*/
const double HOME_ANGLE = 90.00;

/*--------------------------------------------------/
/                   Structures                      /
/--------------------------------------------------*/
static struct option long_options[] =
{
    {"direction",            required_argument, NULL, 'd'},
    {"help",                 no_argument,       NULL,  0},
    {NULL, 0, NULL, 0}
};

/*--------------------------------------------------/
/                   Prototypes                      /
/--------------------------------------------------*/
static void camera_control_print_usage(int es);
static NMT_result camera_control_move_camera(string direction);
static CAM_MOTOR_CTRL_DIRECTIONS camera_control_dir_to_enum(string direction);

using namespace std;
int main (int argc, char *argv[])
{

    /*Initialize Varibles */
    NMT_result result = OK;
    bool       verbosity = false;
    int        longindex = 0;
    int        opt;
    string     dir_str_input; 

    while ((opt = getopt_long(argc, argv, "d:hv", long_options, &longindex)) != -1)
    {
        switch (opt)
        {
            case 'd':
                dir_str_input = optarg;
                break;
            case 'v':
                cout << "Run in verbose mode ................." << endl;
                verbosity = true;
                break;
            case 'h':
                cout << "Help Menu" << endl;
                camera_control_print_usage(0);
                break;
            case '?':
                cout << "Unknown Argument Provided" << endl;
                camera_control_print_usage(0);
                break;
        }
    }

    /*Run Program*/
    NMT_log_init((char *)LOG_DIR, verbosity);
    result = camera_control_move_camera(dir_str_input);

    return result;
}

static void camera_control_print_usage(int es)
{
    cout << "direction/d (UP,DOWN,LEFT,RIGHT,HOME) || -v verbosity || -h/help menu" << endl;
    exit(es);
}

static NMT_result camera_control_move_camera(string dir_str_input)
{
    //Input     : Direction String
    //Output    : N/A
    //Function  : Move camera according to the direction input provided

    /*Initialize Varibles */
    NMT_result result = OK;
    CAM_MOTOR_CTRL_DIRECTIONS direction;

    /* Init Camera Object */
    Camera_Motor_Ctrl camera_control;

    NMT_log_write(DEBUG, (char *)"> direction=%s", dir_str_input.c_str());

    if (dir_str_input.compare("HOME") != 0)
    {
        direction = camera_control_dir_to_enum(dir_str_input);
        result = camera_control.CAM_MTR_CTRL_MOVE_CAMERA(direction);
    }
    else
    {
        direction = CUSTOM;

        /* Move Horizontal Camera to Home Postion */
        result = camera_control.CAM_MTR_CTRL_MOVE_CAMERA(direction, CAM_HRZN_MTR, HOME_ANGLE);
        usleep(1000);

        if (result == OK)
        {
            /* Move Vertical Camera to Home Position */
            result = camera_control.CAM_MTR_CTRL_MOVE_CAMERA(direction, CAM_VERT_MTR, HOME_ANGLE);
        }
    }

    /*Exit the function */
    NMT_log_write(DEBUG, (char *)"< result=%s", result_e2s[result]);
    return result;

}
static CAM_MOTOR_CTRL_DIRECTIONS camera_control_dir_to_enum(string direction)
{
    //Input     : Direction String
    //Output    : Direction ENUM
    //Function  : Convert direction string to enum

    /*Initialize Varibles */
    int size_of_array;
    int dir_enum;

    NMT_log_write(DEBUG, (char *)"> direction=%s", direction.c_str());

    /* Convert String to Enum */
    size_of_array = sizeof(DIRECTION_TO_STR)/sizeof(DIRECTION_TO_STR[0]);
    auto itr = find(DIRECTION_TO_STR, DIRECTION_TO_STR + size_of_array, direction);
    dir_enum = distance(DIRECTION_TO_STR, itr);

    /* Exit the function */
    NMT_log_write(DEBUG, (char *)"< dir_enum=%d", dir_enum);
    return static_cast<CAM_MOTOR_CTRL_DIRECTIONS>(dir_enum);
}
