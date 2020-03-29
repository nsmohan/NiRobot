/** 
 *  @file      L9110.hpp
 *  @brief     Header File for L9110.hpp
 *  @details   External Interfaces for L9110 Driver
 *  @author    Nitin Mohan
 *  @date      March 27, 2019
 *  @copyright 2020 - NM Technologies
 */

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "RSXA.h"


/*--------------------------------------------------/
/                   Constants                       /
/--------------------------------------------------*/
extern const double DEFAULT_SPEED = 50.00;

/** @enum directions
 *  Possible directions the Robot can move */
typedef enum direction {FORWARD, REVERSE, STOP} direction;

/** @var dir_to_str
 *  Facility to convert direction enum to string */
const std::string dir_to_str[] = {"FORWARD", "REVERSE", "STOP"};

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
        void L9110_move_motor(direction dir, double speed=DEFAULT_SPEED);

    private:

        /** @var sim_mode
         *  Simulation Mode of the Hardware */
        bool sim_mode;

        /** @var forward
         *  Pin Mapping for forward GPIO */
        int  forward;

        /** @var reverse
         *  Pin Mapping for reverse GPIO */
        int  reverse;
};

