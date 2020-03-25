/** 
 *  @file      RMDR.hpp
 *  @brief     Header File for RMDR.cpp
 *  @details   External Interfaces for RMDR.cpp
 *  @author    Nitin Mohan
 *  @date      March 17, 2020
 *  @copyright 2020 - NM Technologies
 */

#ifndef DEF_RMDR
#define DEF_RMDR

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_stdlib.h"


/** @class RMDR
 *  Robot Motor Driver Object */
class RMDR
{
    public:

        /* Constructor  */
        RMDR();

        /* Prototypes */
        NMT_result RMDR_driver_status(bool *initialized);

    private:

        /** @var wpi_fd
         *  file pointer for I2C Access to PWM Driver */
        int wpi_fd;

        /** @var result
         *  Overall state of object */
        NMT_result result;

        /** @var pwm_driver_mode
         *  PWM Driver Mode (Sim/Real) */
        bool pwm_driver_mode = false;

        /* Prototypes */
        NMT_result RMDR_initialize_motors();
        NMT_result RMDR_seti2c();
};
#endif
