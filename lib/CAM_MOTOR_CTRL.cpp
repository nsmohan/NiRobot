/** 
 *  @file      CAM_MOTOR_CTRL.cpp
 *  @brief     Interface for moving the camera motors
 *  @details   This libraries provides methods to move the motor
 *  @author    Nitin Mohan
 *  @date      Feb 7, 2019
 *  @copyright 2020 - NM Technologies
 */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <iostream>
#include <cstring>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "CAM_MOTOR_CTRL.hpp"
#include "MTDR.h"

using namespace std;
Camera_Motor_Ctrl::Camera_Motor_Ctrl(RSXA RSXA_Object)
{
    /*!
     *  @brief    Constructor for Camera_Motor_Ctrl
     *  @return   None
     */

    /* Initialize Varibles */
    this->horizontal_motor = CAM_HRZN_MTR;
    this->vertical_motor = CAM_VERT_MTR;
    this->settings = {0};
    this->RSXA_Object = RSXA_Object;

}

Camera_Motor_Ctrl::~Camera_Motor_Ctrl()
{
    /*!
     *  @brief    Destructor for Camera_Motor_Ctrl
     *  @return   None
     */

}

NMT_result Camera_Motor_Ctrl::CAM_MTR_CTRL_MOVE_CAMERA(CAM_MOTOR_CTRL_DIRECTIONS direction, 
                                                       string motor,
                                                       double angle_to_move, 
                                                       double default_angle)
{
    /*!
     *  @brief    Move the camera as per direction input provided
     *  @param[in] direction
     *  @param[in] motor (Optional)
     *  @param[in] angle_to_move (Optional)
     *  @param[in] default_angle (Optional)
     *  @return    NMT_result
     */

    /*Initialize Varibles */
    NMT_result result = OK;
    double     angle;
    bool       sim_mode;
    bool       initialized = false;


    NMT_log_write(DEBUG, (char *)"> direction=%s, angle_to_move=%.2f, default_angle=%.2f",
                  DIRECTION_TO_STR[direction].c_str(), angle_to_move, default_angle);

    /* Check if we're in simulation mode */
    for (int i = 0; i < this->RSXA_Object.array_len_hw; i++)
    {
       if (strcmp(PCA9685_HW_NAME, this->RSXA_Object.hw[i].hw_name) == 0)
       {
           sim_mode = this->RSXA_Object.hw[i].hw_sim_mode;
       } 
    }

    NMT_log_write(DEBUG, (char *)"hw_name=%s sim_mode=%s", 
                  PCA9685_HW_NAME, btoa(sim_mode));

    /* Initialize I2C Communication if we're not in sim mode */
    if ((!sim_mode) && (result == OK))
    {
        result = MTDR_seti2c(&(this->settings));
    }
    
    /* Get the status of the PCA9685 Driver */
    if (result == OK)
    {
        result = MTDR_get_pca9685_status(&(this->settings), &initialized, sim_mode);
    }

    if ((result == OK) && (!initialized))
    {
        result = MTDR_init(&(this->settings), sim_mode);
    }

    if (result == OK)
    {
        switch (direction)
        {
            case UP:
                motor = this->vertical_motor;
                result = MTDR_get_current_position((char *)motor.c_str(),
                                                    &angle, 
                                                    &(this->settings),
                                                    sim_mode);

                angle_to_move = angle + default_angle;
                break;
            case DOWN:
                motor = this->vertical_motor;
                result = MTDR_get_current_position((char *)motor.c_str(),
                                                    &angle,
                                                    &(this->settings),
                                                    sim_mode);

                angle_to_move = angle - default_angle;
                break;
            case LEFT:
                motor = this->horizontal_motor;
                result = MTDR_get_current_position((char *)motor.c_str(),
                                                    &angle,
                                                    &(this->settings),
                                                    sim_mode);

                angle_to_move = default_angle + angle;
                break;
            case RIGHT:
                motor = this->horizontal_motor;
                result = MTDR_get_current_position((char *)motor.c_str(),
                                                    &angle,
                                                    &(this->settings),
                                                    sim_mode);

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
        result = MTDR_move_motor((char *)motor.c_str(),
                                 angle_to_move,
                                 &(this->settings),
                                 sim_mode);
    }

    /* Exit the Function */
    NMT_log_write(DEBUG, (char * )"< result=%s", result_e2s[result]);
    return result; 
}
