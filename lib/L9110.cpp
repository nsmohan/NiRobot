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
#include <wiringPi.h>

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

    /* Get the hardware Name and mode */
    this->hw_name = hw_config.hw_name;
    this->sim_mode = hw_config.hw_sim_mode;

    /* Find and fill the forward/reverse pins */
    for (unsigned int i = 0; i < MAX_MOTORS; i++)
    {
        if (strcmp(hw_config.hw_interface[i].pin_name, "forward") == 0)
        {
            this->forward = hw_config.hw_interface[i].pin_no;
        }

        else if (strcmp(hw_config.hw_interface[i].pin_name, "reverse") == 0)
        {

            this->reverse = hw_config.hw_interface[i].pin_no;
        }
        else
        {
            NMT_log_write(ERROR, (char *)"Forward or Reverse pinNo not found for %s", this->hw_name.c_str());
            throw std::runtime_error("Invalid PinName Found!");
        }
    }

    /* Initialize Hardware Pins */
    if (!(this->sim_mode))
    {
        /* Initialize Wiring Pi */
        wiringPiSetup();

        /* Set Pin Modes */
        pinMode(this->forward, OUTPUT);
        pinMode(this->reverse, OUTPUT);

        /* Set Outputs to low so motors dont move */
        digitalWrite(this->forward, LOW);
        digitalWrite(this->reverse, LOW);

        /* Allow everything to settle */
        delay(SETTLE_TIME);
    }

    /* Exit the function */
    NMT_log_write(DEBUG, (char *)"< hw_name=%s sim_mode=%s forward=%d reverse=%d",
                                 this->hw_name.c_str(), btoa(this->sim_mode), this->forward, this->reverse);
} 

void L9110::L9110_move_motor(L9110_DIRECTIONS direction, double speed)
{
    /*!
     *  @brief     Move the motor at the desired speed
     *  @param[in] dir
     *  @param[in] speed (Optional)
     *  @return    void
     */

    NMT_log_write(DEBUG, (char *)" > dir=%s speed=%.2f", L9110_DIR_TO_STR[direction].c_str(), speed);

    if (!(this->sim_mode))
    {
        switch (direction)
        {
            case FORWARD:
                digitalWrite(this->forward, speed);
                break;
            case REVERSE:
                digitalWrite(this->reverse, speed);
                break;
            case STOP:
                digitalWrite(this->forward, 0.00);
                digitalWrite(this->reverse, 0.00);
                break;
        }
        delay(SETTLE_TIME);
    }
    NMT_log_write(DEBUG, (char *)"< ");
}

