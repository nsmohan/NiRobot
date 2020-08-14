/** 
 *  @file      MTDR.c
 *  @brief     LD27MG Camera Motor Drivers
 *  @details   Low level driver library to control the camera motors
 *  @author    Nitin Mohan
 *  @date      Feb 7, 2019
 *  @copyright 2020 - NM Technologies
 */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_log.h"
#include "MTDR.h"
#include "RSXA.h"

//---------------------Macros----------------------//

/** @def MAX_ANGLE 
 * Maximum angle motor is allowed to move */
#define MAX_ANGLE     180.00

/** @def MIN_ANGLE 
 * Minimum angle motor is allowed to move */
#define MIN_ANGLE     0.00

/** @def HOME_ANGLE 
 * Home angle motor */
#define HOME_ANGLE     90.00

/** @def LD27MG_FREQ
 *  Frequency the LD27MG Motors operate at */
#define LD27MG_FREQ   50.00

/** @def LD27MG_OFFSET
 *  Y-Intercept (setting for angle = 0.00) */
#define LD27MG_OFFSET 0.5

/** @def LD27MG_SLOPE
 * Slope. Rate of change in time for angle */
#define LD27MG_SLOPE  135

/**@def PCA9685_I2C_ADDRESS
 * PCA9685 I2C Address */
#define PCA9685_I2C_ADDRESS 0x40

/*--------------------------------------------------/
/                   Structures                      /
/--------------------------------------------------*/

//------------------Prototypes-------------------//
static NMT_result mtdr_m2c(char *motor, PCA9685_PWM_CHANNEL *channel);
static double     mtdr_get_duty_cycle(double angle);
static double     mtdr_get_angle(double duty_cycle);

NMT_result MTDR_get_current_position(char *motor, double *angle, 
                                     PCA9685_settings *settings,
                                     bool sim_mode)
{
    /*!
     *  @brief     Get the current duty cycle of PWM Channel
     *             and calcualte the angle
     *  @param[in] motor
     *  @param[in] settings
     *  @param[in] sim_mode
     *  @param[out] angle
     *  @return    NMT_result
     */
    
    NMT_log_write(DEBUG, "> motor=%s", motor);

    /* Initialize varibles */
    NMT_result result = OK;
    PCA9685_PWM_CHANNEL channel;

    /* Get corresponding channel to motor string */
    result = mtdr_m2c(motor, &channel);

    /* Get duty cycle */
    if (result == OK)
    {
        result = PCA9685_getPWM(settings, channel, sim_mode);
    }
    else
    {
        NMT_log_write(ERROR, "Unable to find motor=%s in list of known motors", motor);
    }

    if (result == OK)
    {
        *angle = mtdr_get_angle(settings->duty_cycle);
    }

    NMT_log_write(DEBUG, "< angle=%.2f result=%s", *angle, result_e2s[result]);
    return result;
}

NMT_result MTDR_move_motor(char *motor, double angle,
                           PCA9685_settings *settings,
                           bool sim_mode)
{
    /*!
     *  @brief     Get the duty cycle needed to move to motor to the
     *             provided angle and send to PCA9685.c
     *  @param[in] motor
     *  @param[in] angle
     *  @param[in] settings
     *  @param[in] sim_mode
     *  @return    NMT_result
     */

    NMT_log_write(DEBUG, "> motor=%s angle=%f", motor, angle);

    /* Initialize varibles */
    NMT_result result = OK;
    PCA9685_PWM_CHANNEL channel;

    /* Get corresponding channel to motor string */
    result = mtdr_m2c(motor, &channel);

    /* Set PWM for the corresponding channel */
    if (result == OK)
    {
        settings->duty_cycle = mtdr_get_duty_cycle(angle);
        result = PCA9685_setPWM(settings, channel, sim_mode);
    }
    else
    {
        NMT_log_write(ERROR, "Unable to find motor=%s in list of known motors", motor);
    }

    NMT_log_write(DEBUG, "< result=%s",result_e2s[result]);
    return result;
}

NMT_result MTDR_get_pca9685_status(PCA9685_settings *settings,
                                   bool *initialized,
                                   bool sim_mode)
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

    NMT_log_write(DEBUG, "> fd=%d", settings->fd);

    /* Get the status from hardware */
    settings->freq = LD27MG_FREQ;
    PCA9685_get_init_status(settings, initialized, sim_mode);

    /* Exit the function */
    NMT_log_write(DEBUG, "< initialized=%s result=%s", btoa(initialized), result_e2s[result]);
    return result;
}

NMT_result MTDR_init(PCA9685_settings *settings, bool sim_mode)
{
    /*!
     *  @brief     Initialize the PCA9685 Driver and move the 
     *             motors to the home ppsition
     *  @param[in] settings
     *  @param[in] sim_mode
     *  @return    NMT_result
     */

    /* Initialize Variables */
    NMT_result result = OK;

    NMT_log_write(DEBUG, "> fd=%d", settings->fd);

    settings->freq = LD27MG_FREQ;
    settings->delay_time = 0;
    settings->duty_cycle = mtdr_get_duty_cycle(HOME_ANGLE);

    /* Initialize the PCA9685 Driver */
    result = PCA9685_init(settings, sim_mode);

    /* Move the LD27MG Motors to home position */
    for (int i = 0; i < MAX_NR_OF_MOTORS; i++)
    {
        if (result == OK)
        {
            result = PCA9685_setPWM(settings,
                                    MTDR_motors[i].channel,
                                    sim_mode);
        }
    }

    NMT_log_write(DEBUG, "< result=%s", result_e2s[result]);
    return result;
}

NMT_result MTDR_seti2c(PCA9685_settings *settings)
{
    /*!
     *  @brief     Initialize I2C communication with PCA9685 Driver
     *  @param[out] settings
     *  @return    NMT_result
     */

    //Initialize Varibles
    NMT_result result  = OK;

    NMT_log_write(DEBUG, ">");

    //Initialize I2C Communication
    wiringPiSetup();
    settings->fd = wiringPiI2CSetup(PCA9685_I2C_ADDRESS);

    //Check if found the slave address
    if (settings->fd < 0)
    {
        NMT_log_write(ERROR, "I2C Init Failed");
        return result = NOK;
    }
    
    NMT_log_write(DEBUG, "< result=%s", result_e2s[result]);
    return result;
}

static NMT_result mtdr_m2c(char *motor, PCA9685_PWM_CHANNEL *channel)
{
    //Input     : String with motor name
    //Output    : Channel the motor name corresponds to
    //Function  : Convert motor string to appropriate channel

    /*!
     *  @brief     Convert motor string to motor channel
     *  @param[in] motor
     *  @param[out] channel
     *  @return    NMT_result
     */
    NMT_log_write(DEBUG, "> motor=%s", motor);

    /* Initialize Variables */
    NMT_result result = OK;
    int motor_size = sizeof(MTDR_motors)/sizeof(MTDR_motors[0]);

    /* Search for the name in struct */
    for (int i = 0; i < motor_size; i++)
    {
        if (!strcmp(motor, MTDR_motors[i].motor))
        {
            *channel = MTDR_motors[i].channel;
            result = OK;
            NMT_log_write(DEBUG, "< channel=%s result=%s",
                          PCA9685_PWM_CHANNEL_e2s[*channel], result_e2s[result]);
            return result;
        }
    }

    /*Exit the function */
    result = NOK;
    NMT_log_write(DEBUG, "< result=%s",result_e2s[result]);
    return result;
}

static double mtdr_get_duty_cycle(double angle)
{
    /*!
     *  @brief     Convert Duty Cycle based on angle
     *  @param[in] angle
     *  @return    duty_cycle
     */
    NMT_log_write(DEBUG, "> angle: %f",angle);
            
    //Initialize Varibles
    double on_time;
    double off_time;
    double duty_cycle;

    //Ensure the provided angle is within limits
    angle = (angle > MAX_ANGLE ? MAX_ANGLE : (angle < MIN_ANGLE ? MIN_ANGLE : angle));

    //on_time  (ms) = (angle (degrees * SLOPE) + OFFSET
    //off_time (ms) = (1/FREQ) * 1000
    on_time  = (angle/LD27MG_SLOPE) + LD27MG_OFFSET;
    off_time = ((1/LD27MG_FREQ) * 1000);

    duty_cycle = ((on_time/off_time) * 100);

    //Exit the function
    NMT_log_write(DEBUG, "< on_time: %.2fms off_time: %.2fms duty_cycle: %.2f%%",on_time, off_time, duty_cycle);
    return duty_cycle;
}

static double mtdr_get_angle(double duty_cycle)
{
    /*!
     *  @brief     Calculate angle based on duty cycle
     *  @param[in] duty_cycle
     *  @return    angle
     */

    NMT_log_write(DEBUG, "> angle: %f",duty_cycle);
            
    //Initialize Varibles
    double on_time;
    double off_time;
    double angle;

    //off_time (ms) = (1/FREQ) * 1000
    off_time = ((1/LD27MG_FREQ) * 1000);
    on_time = (duty_cycle/100) * off_time;

    angle = (on_time - LD27MG_OFFSET) * LD27MG_SLOPE;

    //Exit the function
    NMT_log_write(DEBUG, "< on_time: %.2fms off_time: %.2fms angle: %.2f",on_time, off_time, angle);

    return angle;
}
