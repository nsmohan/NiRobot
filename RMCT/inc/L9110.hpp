/** 
 *  @file      L9110.hpp
 *  @brief     Header File for L9110.hpp
 *  @details   External Interfaces for L9110 Driver
 *  @author    Nitin Mohan
 *  @date      March 27, 2019
 *  @copyright 2020 - NM Technologies
 */

#ifndef _L9110_
#define _L9110_

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "PCA9685.h"

/*--------------------------------------------------/
/                   Other Component Imports         /
/--------------------------------------------------*/
#include "RSXA/RSXA.h"

/*--------------------------------------------------/
/                   Constants                       /
/--------------------------------------------------*/
/** @var DEFAULT_SPEED
 *  Default motor speed the robot moves at */
const double DEFAULT_SPEED = 50;

/** @enum directions
 *  Possible directions the Robot can move */
typedef enum {FORWARD, REVERSE, STOP} L9110_DIRECTIONS;

/** @var dir_to_str
 *  Facility to convert direction enum to string */
const std::string L9110_DIR_TO_STR[] = {"FORWARD", "REVERSE", "STOP"};

/** @class L9110 
 *  Driver Object */
class L9110
{
    public:
        /* @var hw_name
         * Name of the Motor */
        std::string hw_name;

        /* Constructor */ 
        L9110(RSXA_hw hw_config);

        /* Destructor */
        ~L9110() {}

        /* Prototypes */
        NMT_result L9110_move_motor(L9110_DIRECTIONS direction, int speed=DEFAULT_SPEED);

    private:
        /** @var sim_mode
         *  Simulation Mode of the Hardware */
        bool sim_mode;

        /** @var forward
         *  Pin Mapping for forward PWM Channel */
        PCA9685_PWM_CHANNEL  forward;

        /** @var reverse
         *  Pin Mapping for reverse PWM CHannel */
        PCA9685_PWM_CHANNEL  reverse;
};
#endif
