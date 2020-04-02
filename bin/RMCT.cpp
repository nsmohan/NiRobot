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

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "RSXA.h"
#include "NMT_log.h"
#include "LD27MG.h"
#include "L9110.hpp"

/*--------------------------------------------------/
/                    Macros                         /
/--------------------------------------------------*/
/** @var MY_NAME
 *  Name of this process */
#define MY_NAME "RMCT"

/*--------------------------------------------------/
/                   Global Varibles                 /
/--------------------------------------------------*/
/** @var NO_OF_HW
 *  Quantity of Hardware RMCT directly controls*/
const unsigned int NO_OF_HW = 3;

/** @var PWM_FREQ
 *  PWM Frequency for PCA9685 Driver */
const float PWM_FREQ = 50.00;

/**@var LEFT_MOTOR
 * Name of the Left Drive Motor */
const std::string LEFT_MOTOR = "LEFT_DRV_MTR";

/**@var RIGHT_MOTOR
 * Name of the Right Drive Motor */
const std::string RIGHT_MOTOR = "RIGHT_DRV_MTR";

/** @struct CAMERA_MOTOR_STR_TO_ENUM
 *  Convert RMCT Motor names to driver name */
const struct
{
    /** RMCT_camera_motor
     *  Name of RMCT_ Camera Motor */
    std::string RMCT_camera_motor;

    /** @var ld27mg_camera_motor
     *  Name of Camera Motor for LD27MG Driver */
    LD27MG_MOTORS ld27mg_camera_motor;

}CAMERA_MOTOR_STR_TO_ENUM[] =
{
    {"Camera_Horizontal_Motor", CAM_HRZN_MTR},
    {"Camera_Vertical_Motor", CAM_VERT_MTR}
};

using namespace std;

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

    /** @var rmct_hw_settings
     *  RMCT Task Settings */
    RSXA_procs rmct_task_config;

} RMCT_hw_settings;

/*--------------------------------------------------/
/                   Structs/Classes/Enums            /
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
/** @class RobotMotorController
 *  Object which controls all Peripherals */
class RobotMotorController
{
    public:
        /* Constructor for RobotMotorController */
        RobotMotorController(RSXA_hw pca9685_hw_config, 
                             RSXA_hw left_motor_hw_config, 
                             RSXA_hw right_motor_hw_config);

        /* Prototypes */
        NMT_result move_camera_motor(CAMERA_MOTOR_DIRECTIONS direction, 
                                            LD27MG_MOTORS camera_motor = CAM_HRZN_MTR, 
                                            double angle_to_move = 0.00, 
                                            double default_angle = camera_motor_sensitivity);

   private:
        /** @var motor_sensitivity 
         * The default amount the motor should when direction is provided */
        static const int camera_motor_sensitivity = 10.00;
        
        /** @var default_drive_motor_speed 
         *  Default Speed the Drive Motors move */
        static const int default_drive_motor_speed = 50.00;

   protected:
        /** @var left_drv_motor
         *  Left Drive Motor Object */
        L9110 left_drv_motor;

        /** @var right_drv_motor
         *  Right Drive Motor Object */
        L9110 right_drv_motor;
};

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
    if (result == OK)
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


static NMT_result rmct_get_robot_settings(RSXA &hw_settings, RMCT_hw_settings &rmct_hw_settings);
int main()
{
    /*!
     *  @brief     Main entry point for RMCT
     *  @return    NMT_result
     */

    /** Initialize Varibles */
    RMCT_hw_settings RMCT_hw_settings = {0};
    RSXA hw_settings = {0};
    NMT_result result = OK;

    cout << "Starting Robot Motor Controller ......" << endl;

    /* 1. Get Robot Settings */
    result = RSXA_init(&hw_settings);

    if (result == OK)
        result = rmct_get_robot_settings(hw_settings, RMCT_hw_settings);

    if (result == OK)
    {
        /* Initialize the logger */
        cout << "Initializing the Logger " << hw_settings.log_dir << ".........." << endl;
        NMT_log_init((char *)hw_settings.log_dir, true);

        /* Initialize Robot Motor Controller */
        RobotMotorController rmct_obj(RMCT_hw_settings.pca9685_hw_config,
                                      RMCT_hw_settings.left_motor_hw_config,
                                      RMCT_hw_settings.right_motor_hw_config);

        /** Free RSXA Memory (Everything is initialized) */
        if (result == OK) {RSXA_free_mem(&hw_settings);}
    }

    if (result == OK) {NMT_log_finish();}
    return result;
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
        for (int i = 0; i < hw_settings.array_len_hw; i++)
        {
            if (strcmp(hw_settings.hw[i].hw_name, PCA9685_HW_NAME) == 0)
            {
                memcpy(&(rmct_hw_settings.pca9685_hw_config), &(hw_settings.hw[i]), sizeof(hw_settings.hw[i]));
                mc++;
            }
            else if (strcmp(hw_settings.hw[i].hw_name, LEFT_MOTOR.c_str()) == 0)
            {
                memcpy(&(rmct_hw_settings.left_motor_hw_config), &(hw_settings.hw[i]), sizeof(hw_settings.hw[i]));
                mc++;
            }
            else if (strcmp(hw_settings.hw[i].hw_name, RIGHT_MOTOR.c_str()) == 0)
            {
                memcpy(&(rmct_hw_settings.right_motor_hw_config), &(hw_settings.hw[i]), sizeof(hw_settings.hw[i]));
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

    cout << "Validaton result=" << result_e2s[result] << ".........." << endl;
    return result;
}
