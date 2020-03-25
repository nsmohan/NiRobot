/** 
 *  @file      LD27MG.c
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

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_log.h"
#include "LD27MG.h"
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

/** @def LD27MG_OFFSET
 *  Y-Intercept (setting for angle = 0.00) */
#define LD27MG_OFFSET 0.5

/** @def LD27MG_SLOPE
 * Slope. Rate of change in time for angle */
#define LD27MG_SLOPE  135

/*--------------------------------------------------/
/                   Structures                      /
/--------------------------------------------------*/
/** @struct LD27MG_M2C_MAP
 *  List of Motors mapped to corresponding channel */
const struct
{

    /** @var channel
     *  PWM Channel */
    PCA9685_PWM_CHANNEL channel;

    /** @var motor
     *  motor name */
    LD27MG_MOTORS motor;

} LD27MG_M2C_MAP[] = 
{
    {CHANNEL_15, CAM_HRZN_MTR},
    {CHANNEL_14, CAM_VERT_MTR}
};


/*--------------------------------------------------/
/                   Constants                       /
/--------------------------------------------------*/
/** MAX_NR_OF_MOTORS no. of LD27MG connected */
const int MAX_NR_OF_MOTORS = 2;

/*--------------------------------------------------/
/                   Prototypes                      /
/--------------------------------------------------*/
static PCA9685_PWM_CHANNEL LD27MG_m2c(LD27MG_MOTORS motor);
static double              LD27MG_get_duty_cycle(double angle, double freq);
static double              LD27MG_get_angle(double duty_cycle, double freq);

/*--------------------------------------------------/
/                   Start of Program                /
/--------------------------------------------------*/
NMT_result LD27MG_get_current_position(LD27MG_MOTORS motor, double *angle, 
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
    
    NMT_log_write(DEBUG, "> motor=%s", LD27MG_m2s[motor]);

    /* Initialize varibles */
    NMT_result result = OK;
    PCA9685_PWM_CHANNEL channel;

    /* Get corresponding channel to motor string */
    channel = LD27MG_m2c(motor);

    /* Get duty cycle */
    result = PCA9685_getPWM(settings, channel, sim_mode);

    if (result == OK)
    {
        *angle = LD27MG_get_angle(settings->duty_cycle, settings->freq);
    }

    NMT_log_write(DEBUG, "< angle=%.2f result=%s", *angle, result_e2s[result]);
    return result;
}

NMT_result LD27MG_move_motor(LD27MG_MOTORS motor, double angle,
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

    NMT_log_write(DEBUG, "> motor=%s angle=%f", LD27MG_m2s[motor], angle);

    /* Initialize varibles */
    NMT_result result = OK;
    PCA9685_PWM_CHANNEL channel;

    /* Get corresponding channel to motor string */
    channel = LD27MG_m2c(motor);

    /* Set PWM for the corresponding channel */
    settings->duty_cycle = LD27MG_get_duty_cycle(angle, settings->freq);
    
    settings->delay_time = 0;
    result = PCA9685_setPWM(settings, channel, sim_mode);

    NMT_log_write(DEBUG, "< result=%s",result_e2s[result]);
    return result;
}


NMT_result LD27MG_init(PCA9685_settings *settings, bool sim_mode) 
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

    settings->delay_time = 0;
    settings->duty_cycle = LD27MG_get_duty_cycle(HOME_ANGLE, settings->freq);

    /* Move the LD27MG Motors to home position */
    for (int i = 0; i < MAX_NR_OF_MOTORS; i++)
    {
        if (result == OK)
        {
            result = PCA9685_setPWM(settings,
                                    LD27MG_M2C_MAP[i].channel,
                                    sim_mode);
        }
    }

    NMT_log_write(DEBUG, "< result=%s", result_e2s[result]);
    return result;
}


static PCA9685_PWM_CHANNEL LD27MG_m2c(LD27MG_MOTORS motor)
{
    //Input     : String with motor name
    //Output    : Channel the motor name corresponds to
    //Function  : Convert motor string to appropriate channel

    /*!
     *  @brief     Convert motor string to motor channel
     *  @param[in] motor
     *  @return    channel
     */
    NMT_log_write(DEBUG, "> motor=%s", LD27MG_m2s[motor]);

    /* Initialize Variables */
    PCA9685_PWM_CHANNEL channel;
    int motor_size = sizeof(LD27MG_M2C_MAP)/sizeof(LD27MG_M2C_MAP[0]);

    /* Search for the name in struct */
    for (int i = 0; i < motor_size; i++)
    {
        if (motor == LD27MG_M2C_MAP[i].motor)
        {
            /* Channel Found */
            channel = LD27MG_M2C_MAP[i].channel;
            break;
        }
    }

    /* Exit the Function */
    NMT_log_write(DEBUG, "< channel=%s", PCA9685_PWM_CHANNEL_e2s[channel]);
    return channel;
}

static double LD27MG_get_duty_cycle(double angle, double freq)
{
    /*!
     *  @brief     Convert Duty Cycle based on angle
     *  @param[in] angle
     *  @param[in] freq
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
    off_time = ((1/freq) * 1000);

    duty_cycle = ((on_time/off_time) * 100);

    //Exit the function
    NMT_log_write(DEBUG, "< on_time: %.2fms off_time: %.2fms duty_cycle: %.2f%%",on_time, off_time, duty_cycle);
    return duty_cycle;
}

static double LD27MG_get_angle(double duty_cycle, double freq)
{
    /*!
     *  @brief     Calculate angle based on duty cycle
     *  @param[in] duty_cycle
     *  @param[in] freq
     *  @return    angle
     */

    NMT_log_write(DEBUG, "> angle: %f",duty_cycle);
            
    //Initialize Varibles
    double on_time;
    double off_time;
    double angle;

    //off_time (ms) = (1/FREQ) * 1000
    off_time = ((1/freq) * 1000);
    on_time = (duty_cycle/100) * off_time;

    angle = (on_time - LD27MG_OFFSET) * LD27MG_SLOPE;

    //Exit the function
    NMT_log_write(DEBUG, "< on_time: %.2fms off_time: %.2fms angle: %.2f",on_time, off_time, angle);

    return angle;
}
