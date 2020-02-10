/** 
 *  @file      CAM_MOTOR_CTRL.hpp
 *  @brief     Header file for CAM_MOTOR_CTRL.cpp
 *  @details   External interfaces to control the camera motors
 *  @author    Nitin Mohan
 *  @date      Feb 7, 2019
 *  @copyright 2020 - NM Technologies
 */


#ifndef DEF_CAM_MOTOR_CTRL
#define DEF_CAM_MOTOR_CTRL


/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <iostream>
#include <string>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_stdlib.h"
#include "PCA9685.h"
#include "RSXA.h"

/*--------------------------------------------------/
/                   Globals                         /
/--------------------------------------------------*/
/** @var MOTOR_SENSITIVITY
 *  The angle the motor moves with from its current position when
 *  instructed to move in any direction. */
const int MOTOR_SENSITIVITY = 10;


/**@var DIRECTION_TO_STR
 * Used to convert direction enum to string */
extern const std::string DIRECTION_TO_STR[5] = {"UP",
                                                "DOWN", 
                                                "LEFT",
                                                "RIGHT", 
                                                "CUSTOM"};


/*--------------------------------------------------/
/                   Structs/Classes/Enums            /
/--------------------------------------------------*/
/** @enum CAM_MOTOR_CTRL_DIRECTIONS
 *  Possible directions the motor can move */
typedef enum {UP, 
              DOWN, 
              LEFT,
              RIGHT, 
              CUSTOM} CAM_MOTOR_CTRL_DIRECTIONS;


/** @class Camera_Motor_Ctrl
 *  Camera Control Object */
class Camera_Motor_Ctrl
{
    public:

        /** @var horizontal_motor 
         * Holds name of the Horizontal motor */
        std::string horizontal_motor;

        /** @var vertical_motor
         * Holds the name of the Vertical Motor */
        std::string vertical_motor;

        /** @var settings
         * PCA9685 Settings structure */
        PCA9685_settings settings;

        /** @var hw_settings_obj
         * RSXA Settings structure */
        RSXA_hw hw_settings_obj;

        Camera_Motor_Ctrl();

        NMT_result CAM_MTR_CTRL_MOVE_CAMERA(CAM_MOTOR_CTRL_DIRECTIONS direction, 
                                            std::string motor = "", 
                                            double angle_to_move = 0.00, 
                                            double default_angle = MOTOR_SENSITIVITY);
};
#endif
