/** 
 *  @file      RMCT_lib.hpp
 *  @brief     Header file for RMCT_lib.cpp
 *  @details   Robot Motor Controller Library
 *  @author    Nitin Mohan
 *  @date      April 05, 2020
 *  @copyright 2020 - NM Technologies
 */
#ifndef _RCMT_lib_
#define _RCMT_lib_
/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <string>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "LD27MG.h"
#include "L9110.hpp"

/*--------------------------------------------------/
/                   Global Varibles                 /
/--------------------------------------------------*/
/**@var LEFT_DRV_MTR
 * Name of the Left Drive Motor */
const std::string LEFT_DRV_MTR = "LEFT_DRV_MTR";

/**@var RIGHT_DRV_MTR
 * Name of the Right Drive Motor */
const std::string RIGHT_DRV_MTR = "RIGHT_DRV_MTR";

/*--------------------------------------------------/
/                   Structs/Classes/Enums           /
/--------------------------------------------------*/

/** @enum CAM_MOTOR_CTRL_DIRECTIONS
 *  Possible directions the motor can move */
typedef enum {UP, 
              DOWN, 
              LEFT,
              RIGHT, 
              CUSTOM,
              MAX_DIRECTIONS} CAMERA_MOTOR_DIRECTIONS;

/**@var DIRECTION_TO_STR
 * Used to convert direction enum to string */
extern const std::string DIRECTION_TO_STR[MAX_DIRECTIONS] = {"UP",
                                                             "DOWN", 
                                                             "LEFT",
                                                             "RIGHT", 
                                                             "CUSTOM"};
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

    /** @var cam_mtr_step_size
     *  Default amount camera should move */
    double cam_mtr_step_size;

    /** @var default_drive_speed
     *  Default drive speed for motors */
    double default_drive_speed;
} RMCT_hw_settings;

/** @class RobotMotorController
 *  Object which controls all Peripherals */
class RobotMotorController
{
    public:
        /* Constructor for RobotMotorController */
        RobotMotorController(RMCT_hw_settings rmct_hw_settings);

        ~RobotMotorController() {};


        /* Prototypes */
        NMT_result process_motor_action(std::string motor, std::string direction, double angle, int speed);

   private:
        /** @var cam_mtr_step_size 
         * The default amount the motor should when direction is provided */
        double cam_mtr_step_size = 0;

        /** @var default_drive_speed 
         * Default drive speed for camera motors */
        double default_drive_speed = 0;
        
        /** @var default_drive_motor_speed 
         *  Default Speed the Drive Motors move */
        static const int default_drive_motor_speed = 50.00;

        /* Prototypes */
        NMT_result move_camera_motor(CAMERA_MOTOR_DIRECTIONS direction, 
                                     LD27MG_MOTORS camera_motor = CAM_HRZN_MTR, 
                                     double angle_to_move = 0.00);

        virtual L9110* Create_drv_motor(RSXA_hw hw_settings)
        {
            return new L9110(hw_settings);
        }

    protected:
    /** @var left_drv_motor
     *  Left Drive Motor Object */
    L9110 *left_drv_motor;

    /** @var right_drv_motor
     *  Right Drive Motor Object */
    L9110 *right_drv_motor;
};
#endif
