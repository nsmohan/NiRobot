/** 
 *  @file      RMDR.cpp
 *  @brief     Robot Motor Hardware Abstraction Layer
 *  @details   Library to facilitate all communication to MTDR_motors
 *  @author    Nitin Mohan
 *  @date      March 14, 2020
 *  @copyright 2020 - NM Technologies
 */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <iostream>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "RMDR.hpp"
#include "PCA9685.h"
#include "LD27MG.h"

/*--------------------------------------------------/
/                   Constants                       /
/--------------------------------------------------*/
/** @var PWM_FREQ 
 *  Frequency the PWM Driver Operates at */
const double PWM_FREQ = 50.00;

RMDR::RMDR()
{
    /*!
     *  @brief      Constructor for RMDR Object
     *  @return     N/A
     */

    /* Intialize the Motors */
    this->result = RMDR::RMDR_initialize_motors();

    /* Ensure All is Ok, Else throw Error */
    if (result != OK) {throw result;}
}

NMT_result RMDR::RMDR_initialize_motors()
{
    /*!
     *  @brief      Function to initialize the PWM Driver and LD27MG + Drive Motors
     *  @return    NMT_result
     */

    /* Initialized Varibles */
    NMT_result result = OK;
    PCA9685_settings hw_settings = {0};
    hw_settings.freq = PWM_FREQ;
    hw_settings.sim_mode = true;
    
    if (result == OK)
    {
        /* Initialize PWM Driver */
        result = PCA9685_init(hw_settings);
    }

    if (result == OK)
    {
        /* Initialize the Camera Motors */
        result = LD27MG_init();
    }

    return result;
}

NMT_result RMDR::RMDR_driver_status(bool *initialized)
{
    /*!
     *  @brief      Get the status of PCA9685 (Initialized || Not Initialized)
     *  @param[in]  settings
     *  @param[in]  sim_mode
     *  @param[out] initialized
     *  @return     NMT_result
     */

    /* Initialize Varibles */
    NMT_result result      = OK;

    NMT_log_write(DEBUG, (char *)"> ");

    result = PCA9685_get_init_status(initialized);

    /* Exit the function */
    NMT_log_write(DEBUG, (char *)"< initialized=%s result=%s", btoa(initialized), result_e2s[result]);
    return result;
}

