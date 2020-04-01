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

const unsigned int MAX_HW = 3;
const float PWM_FREQ = 50.00;

const std::string LEFT_MOTOR = "LEFT_DRV_MTR";
const std::string RIGHT_MOTOR = "RIGHT_DRV_MTR";

using namespace std;

typedef struct RMDR_hw_settings
{
    RSXA_hw pca9685_hw_config;
    RSXA_hw left_motor_hw_config;
    RSXA_hw right_motor_hw_config;
} RMCT_hw_settings;

class RobotMotorController
{
    public:
        RobotMotorController(RSXA_hw pca9685_hw_config, RSXA_hw left_motor_hw_config, RSXA_hw right_motor_hw_config);

   private:
        L9110 left_drv_motor;
        L9110 right_drv_motor;
};

RobotMotorController::RobotMotorController(RSXA_hw pca9685_hw_config, 
                                           RSXA_hw left_motor_hw_config, 
                                           RSXA_hw right_motor_hw_config) : 
                                           left_drv_motor(left_motor_hw_config) 
                                           ,right_drv_motor(right_motor_hw_config)
{
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


static NMT_result rmct_get_robot_settings(RSXA &hw_settings, RMCT_hw_settings &rmct_hw_settings);
int main()
{
    cout << "Starting Robot Motor Controller ......" << endl;
    RMCT_hw_settings RMCT_hw_settings = {0};
    NMT_result result = OK;
    /* 1. Get Robot Settings */
    RSXA hw_settings = {0};
    result = RSXA_init(&hw_settings);

    result = rmct_get_robot_settings(hw_settings, RMCT_hw_settings);

    if (result == OK)
    {
        /* Initialize the logger */
        NMT_log_init((char *)hw_settings.log_dir, true);

        cout << RMCT_hw_settings.left_motor_hw_config.hw_interface[0].pin_name << endl;

        /* Initialize Robot Motor Controller */
        RobotMotorController rmct_obj(RMCT_hw_settings.pca9685_hw_config,
                                      RMCT_hw_settings.left_motor_hw_config,
                                      RMCT_hw_settings.right_motor_hw_config);
        RSXA_free_mem(&hw_settings);
    }


    return 0;
}

static NMT_result rmct_get_robot_settings(RSXA &hw_settings, RMCT_hw_settings &rmct_hw_settings)
{
    NMT_result result = OK;

    int mc = 0;

    /* 2. Extract the HW Settings Needed */
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

            if (mc == MAX_HW) {break;}
        }

        if (mc < 3)
        {
            cout << "ERROR, Missing Hardware Configuration data in RSXA.json file" << endl;
            result = NOK;
        }
    }


    return result;
}
