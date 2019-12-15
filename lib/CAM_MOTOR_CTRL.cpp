/*CAM_MOTOR_CTRL.cpp: Library to Control the Camera Motors

__author__      = "Nitin Mohan
__copyright__   = "Copy Right 2019. NM Technologies" */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <iostream>
#include <string>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "CAM_MOTOR_CTRL.hpp"
#include "PCA9685.h"
#include "MTDR.h"
#include "RSXA.h"


using namespace std;
Camera_Motor_Ctrl::Camera_Motor_Ctrl()
{
    this->horizontal_motor = CAM_HRZN_MTR;
    this->vertical_motor = CAM_VERT_MTR;
}

NMT_result Camera_Motor_Ctrl::CAM_MTR_CTRL_MOVE_CAMERA(CAM_MOTOR_CTRL_DIRECTIONS direction, 
                                                       string motor,
                                                       double angle_to_move, 
                                                       double default_angle)
{
    //Input     : Direction to move, (Optional) -> motor, angle_to_move, default_angle
    //Output    : N/A
    //Function  : Move camera according to the direction input provided

    /*Initialize Varibles */
    NMT_result result = OK;
    double     angle;
    bool       sim_mode;
    bool       initialized;

    PCA9685_settings settings = {0};

    NMT_log_write(DEBUG, (char *)"> direction=%s, angle_to_move=%.2f, default_angle=%.2f",
                  DIRECTION_TO_STR[direction].c_str(), angle_to_move, default_angle);

    /* Check if we're in simulation mode */
    result = RSXA_get_mode((char *)PCA9685_HW_NAME, &sim_mode);
    NMT_log_write(DEBUG, (char *)"hw_name=%s sim_mode=%s", 
                  PCA9685_HW_NAME, btoa(sim_mode));

    /* Initialize I2C Communication if we're not in sim mode */
    if ((!sim_mode) && (result == OK))
    {
        result = MTDR_seti2c(&settings);
    }
    
    /* Get the status of the PCA9685 Driver */
    if (result == OK)
    {
        result = MTDR_get_pca9685_status(&settings, &initialized);
    }

    if ((result == OK) && (!initialized))
    {
        result = MTDR_init(&settings);
    }

    if (result == OK)
    {
        switch (direction)
        {
            case UP:
                motor = this->vertical_motor;
                result = MTDR_get_current_position((char *)motor.c_str(), &angle, &settings);
                angle_to_move = angle + default_angle;
                break;
            case DOWN:
                motor = this->vertical_motor;
                result = MTDR_get_current_position((char *)motor.c_str(), &angle, &settings);
                angle_to_move = angle - default_angle;
                break;
            case LEFT:
                motor = this->horizontal_motor;
                result = MTDR_get_current_position((char *)motor.c_str(), &angle, &settings);
                angle_to_move = default_angle + angle;
                break;
            case RIGHT:
                motor = this->horizontal_motor;
                result = MTDR_get_current_position((char *)motor.c_str(), &angle, &settings);
                angle_to_move = angle - default_angle;
                break;
            case CUSTOM:
                /* Do Nothing */
                break;
        }
    }

    /* Move the actual Motor */
    if (result == OK)
    {
        result = MTDR_move_motor((char *)motor.c_str(), angle_to_move, &settings);
    }

    NMT_log_write(DEBUG, (char * )"< result=%s", result_e2s[result]);
    return result; 
}
