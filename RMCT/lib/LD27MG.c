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
#include "NMTX/NMT_log.h"
#include "LD27MG.h"

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
 * Slope. Rate of change in time for angle (deg/ms) */
#define LD27MG_SLOPE  135

/** @def MAX_NR_OF_MOTORS 
 *  no. of LD27MG connected */
#define MAX_NR_OF_MOTORS 2
/*--------------------------------------------------/
/                   Structures                      /
/--------------------------------------------------*/
/** @struct LD27MG_M2C_MAP
 *  List of Motors mapped to corresponding channel */
struct LD27MG_motor2channel_mapping
{

    /** @var channel
     *  PWM Channel */
    PCA9685_PWM_CHANNEL channel;

    /** @var motor
     *  motor name */
    LD27MG_MOTORS motor;
};

/*--------------------------------------------------/
/                   Constants                       /
/--------------------------------------------------*/

/** @struct LD27MG_M2C_MAP
 *  Motor to PCA9685 Channel Mapping */
struct LD27MG_motor2channel_mapping LD27MG_M2C_MAP[MAX_NR_OF_MOTORS];

/** @var SIM_MODE
 *  Simulatio Mode for LD27MG*/
bool SIM_MODE;
/*--------------------------------------------------/
/                   Prototypes                      /
/--------------------------------------------------*/
static PCA9685_PWM_CHANNEL LD27MG_m2c(LD27MG_MOTORS motor);
static double              LD27MG_get_duty_cycle(double angle, double freq);
static double              LD27MG_get_angle(double duty_cycle, double freq);
static NMT_result          LD27MG_mtr_str2enum(char *mtr_str, LD27MG_MOTORS *mtr_enum);

/*--------------------------------------------------/
/                   Start of Program                /
/--------------------------------------------------*/
NMT_result LD27MG_init(RSXA_hw hw_config) 
{
    /*!
     *  @brief     Initialize the PCA9685 Driver and move the 
     *             motors to the home ppsition
     *  @return    NMT_result
     */

    /* Initialize Variables */
    NMT_result result = OK;
    bool initialized = false;
    LD27MG_MOTORS camera_motor;

    NMT_log_write(DEBUG, "> ");

    /* Proceed only if the PCA965 Driver has been initialized */
    result = PCA9685_get_init_status(&initialized);
    if ((result == OK) && (!initialized))
    {
        NMT_log_write(ERROR, "PCA9685 Driver not initialized=%s!", btoa(initialized));
        result = NOK;
    }

    /* Get the Simulatio mode */
    SIM_MODE = hw_config.hw_sim_mode;

    /* Apply Motor Settings */
    for (int i = 0; ((result == OK) && (i < MAX_NR_OF_MOTORS)); i++)
    {
        /* Convert Motor String to ENUM */
        result = LD27MG_mtr_str2enum(hw_config.hw_interface[i].pin_name, &camera_motor);

        /* Fill Struct */
        if (result == OK)
        {
            LD27MG_M2C_MAP[i].channel = (PCA9685_PWM_CHANNEL)hw_config.hw_interface[i].pin_no;
            LD27MG_M2C_MAP[i].motor = camera_motor;
        }
    }

    /* Initialize the motors */
    if (result == OK)
    {
        double delay_time = 0;
        double duty_cycle = LD27MG_get_duty_cycle(HOME_ANGLE, PCA9685_get_curret_freq());

        /* Move the LD27MG Motors to home position */
        for (int i = 0; i < MAX_NR_OF_MOTORS; i++)
        {
            if ((result == OK) && (!SIM_MODE))
            {
                result = PCA9685_setPWM(duty_cycle, delay_time,
                                        LD27MG_M2C_MAP[i].channel);
            }
        }
    }

    NMT_log_write(DEBUG, "< result=%s", enum2str(result));
    return result;
}

NMT_result LD27MG_get_current_position(LD27MG_MOTORS motor, double *angle)
{
    /*!
     *  @brief     Get the current duty cycle of PWM Channel
     *             and calcualte the angle
     *  @param[in] motor
     *  @param[out] angle
     *  @return    NMT_result
     */
    
    NMT_log_write(DEBUG, "> motor=%s", LD27MG_m2s[motor]);

    /* Initialize varibles */
    NMT_result result = OK;
    PCA9685_PWM_CHANNEL channel;
    double duty_cycle;

    /* Get corresponding channel to motor string */
    channel = LD27MG_m2c(motor);

    /* Get duty cycle */
    result = PCA9685_getPWM(&duty_cycle, channel);

    if (result == OK)
    {
        *angle = LD27MG_get_angle(duty_cycle, PCA9685_get_curret_freq());
    }

    NMT_log_write(DEBUG, "< angle=%.2f result=%s", *angle, enum2str(result));
    return result;
}

NMT_result LD27MG_move_motor(LD27MG_MOTORS motor, double angle)
{
    /*!
     *  @brief     Get the duty cycle needed to move to motor to the
     *             provided angle and send to PCA9685.c
     *  @param[in] motor
     *  @param[in] angle
     *  @return    NMT_result
     */

    NMT_log_write(DEBUG, "> motor=%s angle=%f", LD27MG_m2s[motor], angle);

    /* Initialize varibles */
    NMT_result result = OK;
    PCA9685_PWM_CHANNEL channel;
    double duty_cycle;
    double delay_time;

    /* Get corresponding channel to motor string */
    channel = LD27MG_m2c(motor);

    /* Set PWM for the corresponding channel */
    duty_cycle = LD27MG_get_duty_cycle(angle, PCA9685_get_curret_freq());
    
    delay_time = 0;

    if (!SIM_MODE)
    {
        result = PCA9685_setPWM(duty_cycle, delay_time, channel);
    }

    NMT_log_write(DEBUG, "< result=%s",enum2str(result));
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
    NMT_log_write(DEBUG, "< channel=%s", enum2str(channel));
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

static NMT_result LD27MG_mtr_str2enum(char *mtr_str, LD27MG_MOTORS *mtr_enum)
{
    /*!
     *  @brief     Convert Motor String to LD27MG_MOTORS ENUM
     *  @param[in] mtr_str
     *  @param[in] mtr_enum
     *  @return    NMT_result
     */
    NMT_log_write(DEBUG, "> mtr_str=%s",mtr_str);

    /* Initialize Varibles */
    NMT_result result = OK;

    /* Convert Algorithm */
    if (strcmp(mtr_str, "CAM_HRZN_MTR") == 0)
    {
        *mtr_enum = CAM_HRZN_MTR;
    }
    else if (strcmp(mtr_str, "CAM_VERT_MTR") == 0)
    {
        *mtr_enum = CAM_VERT_MTR;
    }
    else
    {
        result = NOK;
        NMT_log_write(ERROR, "Incorrect LD27MG Motor Name!");
    }

    /* Exit the function */
    NMT_log_write(DEBUG, "< result=%s",enum2str(result));
    return result;
}
