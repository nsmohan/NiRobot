/** 
 *  @file      L9110.cpp
 *  @brief     Driver for L9110 H-Bridge
 *  @details   This library provides interfaces to control the 2 L9110 IC's
 *  @author    Nitin Mohan
 *  @date      March 27, 2019
 *  @copyright 2020 - NM Technologies
 */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <iostream>
#include <chrono>
#include <cstring>
#include <stdexcept>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "L9110.hpp"
#include "NMT_log.h"

/*--------------------------------------------------/
/                   Constants                       /
/--------------------------------------------------*/
static const unsigned int MAX_MOTORS = 2;
static const unsigned int MAX_PINS = 2;
static const double SETTLE_TIME = 500;
static const double DELAY_TIME = 0.00;



/*--------------------------------------------------/
/                   Start of Program                /
/--------------------------------------------------*/
using namespace std;
L9110::L9110(RSXA_hw hw_config)
{
    /*!
     *  @brief     Constructor definition for L9110 Object
     *  @param[in] hw_config RSXA HW Settings
     *  @return    void
     */

    NMT_log_write(DEBUG, (char *)"> ");
    NMT_result result = OK;

    /* Get the hardware Name and mode */
    this->hw_name = hw_config.hw_name;
    this->sim_mode = hw_config.hw_sim_mode;

    /* Find and fill the forward/reverse pins */
    for (unsigned int i = 0; i < MAX_MOTORS; i++)
    {
        if (strcmp(hw_config.hw_interface[i].pin_name, "forward") == 0)
        {
            this->forward = (PCA9685_PWM_CHANNEL)hw_config.hw_interface[i].pin_no;
        }

        else if (strcmp(hw_config.hw_interface[i].pin_name, "reverse") == 0)
        {
            this->reverse = (PCA9685_PWM_CHANNEL)hw_config.hw_interface[i].pin_no;
        }
        else
        {
            NMT_log_write(ERROR, (char *)"Forward or Reverse pinNo not found for %s", this->hw_name.c_str());
            result = NOK;
        }
    }

    /* Initialize Motoros to Stop */
    if (result == OK)
        result = L9110::L9110_move_motor(STOP);

    if (result == NOK)
        throw std::runtime_error("Error! Unable to Initialize L9110 Motors");

    /* Exit the function */
    NMT_log_write(DEBUG, (char *)"< hw_name=%s sim_mode=%s forward=%d reverse=%d",
                                 this->hw_name.c_str(), btoa(this->sim_mode), this->forward, this->reverse);
} 

NMT_result L9110::L9110_move_motor(L9110_DIRECTIONS direction, int speed)
{
    /*!
     *  @brief     Move the motor at the desired speed
     *  @param[in] dir
     *  @param[in] speed (Optional)
     *  @return    void
     */

    NMT_log_write(DEBUG, (char *)"> dir=%s speed=%d", L9110_DIR_TO_STR[direction].c_str(), speed);

    /* Initialize Variables */
    NMT_result result = OK;

    /* Cap Max Speed to 100 and Min to 0 */
    speed = (speed > 100 ? 100 : (speed < 0 ? 0 : speed));

    if (!(this->sim_mode))
    {
        switch (direction)
        {
            case FORWARD:
                if (result == OK) {result = PCA9685_setPWM(0.00, DELAY_TIME, this->reverse);}
                if (result == OK) {result = PCA9685_setPWM((double)speed, DELAY_TIME, this->forward);}
                break;
            case REVERSE:
                if (result == OK) {result = PCA9685_setPWM(0.00, DELAY_TIME, this->forward);}
                if (result == OK) {result = PCA9685_setPWM((double)speed, DELAY_TIME, this->reverse);}
                break;
            case STOP:
                if (result == OK) {result = PCA9685_setPWM(0.00, DELAY_TIME, this->forward);}
                if (result == OK) {result = PCA9685_setPWM(0.00, DELAY_TIME, this->reverse);}
                break;
        }
    }
    NMT_log_write(DEBUG, (char *)"< result=%s", result_e2s[result]);
    return result;
}

