/** 
 *  @file      RMCT_lib.cpp
 *  @brief     Robot Motor Controller Library
 *  @details   Implementation of Robot Motor Library
 *  @author    Nitin Mohan
 *  @date      April 05, 2020
 *  @copyright 2020 - NM Technologies
 */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <iostream>
#include <algorithm>
#include <map>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "RMCT_lib.hpp"
#include "PCA9685.h"
#include "RSXA.h"

/*--------------------------------------------------/
/                   Globals                         /
/--------------------------------------------------*/

/** @map camera_directions
 *  CAMERA_MOTOR_DIRECTIONS STR to ENUM Mapping */
std::map<std::string, CAMERA_MOTOR_DIRECTIONS> camera_directions = {{"UP", UP},
                                                                    {"DOWN", DOWN}, 
                                                                    {"LEFT", LEFT},
                                                                    {"RIGHT", RIGHT},
                                                                    {"CUSTOM", CUSTOM}};

/** @map camera_directions
 *  LD27MG_MOTORS STR to ENUM Mapping */
std::map<std::string, LD27MG_MOTORS> ld27mg_motors = {{"CAM_HRZN_MTR", CAM_HRZN_MTR},
                                                      {"CAM_VERT_MTR", CAM_VERT_MTR}};


/*--------------------------------------------------/
/             Library Implementation                /
/--------------------------------------------------*/
using namespace std;
NMT_result RobotMotorController::process_motor_action(std::string motor, std::string direction, double angle, int speed)
{
    /*!
     *  @brief     Process motor action and send request to
     *             execute it. 
     *  @param[in] motor
     *  @param[in] direction
     *  @param[in] direction
     *  @param[in] speed
     *  @return    NMT_result
     */

    NMT_log_write(DEBUG, (char *)"> motor=%s direction=%s angle=%.2f speed=%d", motor.c_str(), 
                                                                                 direction.c_str(), angle, speed);

    /* Initialize Varibles */
    NMT_result result = OK;

    /* Function Implementation */
    if (ld27mg_motors.count(motor))
    {
        result = move_camera_motor(CUSTOM, ld27mg_motors[motor], angle);
    }
    else if ((camera_directions.count(direction)) && (motor == "CAMERA"))
    {
        result = move_camera_motor(camera_directions[direction]);
    }
    else if (l9110_directions.count(direction))
    {
        if (motor == LEFT_DRV_MTR)
        {
            left_drv_motor.L9110_move_motor(l9110_directions[direction], speed);
        }
        else if (motor == RIGHT_DRV_MTR)
        {
            right_drv_motor.L9110_move_motor(l9110_directions[direction], speed);
        }
    }
    else
    {
        result = NOK;
    }

    /* Exit the function */
    NMT_log_write(DEBUG, (char *)"< result=%s", result_e2s[result]);
    return result;
}

RobotMotorController::RobotMotorController(RSXA_hw pca9685_hw_config, 
                                           RSXA_hw left_motor_hw_config, 
                                           RSXA_hw right_motor_hw_config) : 
                                           left_drv_motor(left_motor_hw_config) 
                                           ,right_drv_motor(right_motor_hw_config)
{
    /*!
     *  @brief     Constructor Implementation for RobotMotorController
     *  @param[in] pca9685_hw_config
     *  @param[in] left_motor_hw_config
     *  @param[in] right_motor_hw_config
     *  @return    void 
     */

    /* Initialize Variables */
    NMT_result result = OK;
    
    /* Initialize the PCA9685 Driver */
    PCA9685_settings pwm_settings = {PWM_FREQ, pca9685_hw_config.hw_sim_mode};
    result = PCA9685_init(pwm_settings);

    /* Initialize the Camera Motors */
    if (result == OK)
        result = LD27MG_init();

    if (result !=OK)
        throw std::runtime_error("ERROR, Robot Motor Hardware Initiization Failed!");
}

NMT_result RobotMotorController::move_camera_motor(CAMERA_MOTOR_DIRECTIONS direction, 
                                                   LD27MG_MOTORS camera_motor,
                                                   double angle_to_move, 
                                                   double default_angle)
{
    /*!
     *  @brief     Method to move the Camera Motors
     *  @param[in] direction
     *  @param[in] camera_motor  (Optional - Manual override)
     *  @param[in] angle_to_move (Optional - Manual override)
     *  @param[in] default_angle (Optional - Change default precison)
     *  @return    NMT_result
     */

    NMT_log_write(DEBUG, (char *)"> direction=%s, angle_to_move=%.2f, default_angle=%.2f",
                  DIRECTION_TO_STR[direction].c_str(), angle_to_move, default_angle);

    /*Initialize Varibles */
    NMT_result result = OK;
    double     angle;

    /** Determine which motor needs to be moved */
    if (direction != CUSTOM)
    {
        switch (direction)
        {
            case UP:
            case DOWN:
                camera_motor = CAM_VERT_MTR;
                break;
            case LEFT:
            case RIGHT:
                camera_motor = CAM_HRZN_MTR;
                break;
            case CUSTOM:
            case MAX_DIRECTIONS:
                break;
        }

        /** Get the Motors current Position */
        result = LD27MG_get_current_position(camera_motor, &angle);
    }

    /** Determine the angle motor should move to */
    if (result == OK)
    {
        switch (direction)
        {
            case UP:
            case LEFT:
                angle_to_move = angle + default_angle;
                break;
            case RIGHT:
            case DOWN:
                angle_to_move = angle - default_angle;
                break;
            case CUSTOM:
            case MAX_DIRECTIONS:
                break;
        }

        /** Move the actual motor */
        result = LD27MG_move_motor(camera_motor, angle_to_move);
    }

    /* Exit the Function */
    NMT_log_write(DEBUG, (char * )"< result=%s", result_e2s[result]);
    return result; 
}