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
#include <wiringPi.h>
#include <wiringPiI2C.h>

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
    
    /* Initialize I2C Communication with the PWM Driver */
    result = RMDR::RMDR_seti2c();
 

    if (result == OK)
    {
        /* Initialize PWM Driver */
        hw_settings.fd = this->wpi_fd;
        result = PCA9685_init(&hw_settings, pwm_driver_mode);
    }

    if (result == OK)
    {
        /* Initialize the Camera Motors */
        result = LD27MG_init(&hw_settings, pwm_driver_mode);
    }

    return result;
}

NMT_result RMDR::RMDR_seti2c()
{
    /*!
     *  @brief     Initialize I2C communication with PCA9685 Driver
     *  @param[out] settings
     *  @return    NMT_result
     */

    //Initialize Varibles
    NMT_result result  = OK;

    NMT_log_write(DEBUG, (char *)">");

    //Initialize I2C Communication
    wiringPiSetup();
    this->wpi_fd = wiringPiI2CSetup(PCA9685_I2C_ADDRESS);

    //Check if found the slave address
    if (this->wpi_fd < 0)
    {
        NMT_log_write(ERROR, (char *)"I2C Init Failed");
        return result = NOK;
    }
    
    NMT_log_write(DEBUG, (char *)"< result=%s", result_e2s[result]);
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
    PCA9685_settings hw_settings = {0};

    NMT_log_write(DEBUG, (char *)"> ");

    /* Get the status from hardware */
    hw_settings.freq = PWM_FREQ;
    hw_settings.fd = this->wpi_fd;

    PCA9685_get_init_status(&hw_settings, initialized, this->pwm_driver_mode);

    /* Exit the function */
    NMT_log_write(DEBUG, (char *)"< initialized=%s result=%s", btoa(initialized), result_e2s[result]);
    return result;
}

