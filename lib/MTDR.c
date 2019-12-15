/*MTDR.c:       Camera Motor Driver

__author__      = "Nitin Mohan
__copyright__   = "Copy Right 2019. NM Technologies" */

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
#define LOG_DIR       "/var/log/NiRobot"
#define MAX_ANGLE     180.00
#define MIN_ANGLE     0.00
#define LD27MG_FREQ   50.00
#define LD27MG_OFFSET 0.5
#define LD27MG_SLOPE  135

//I2C address of PCA9685
#define I2C_ADDRESS 0x40

/*--------------------------------------------------/
/                   Structures                      /
/--------------------------------------------------*/

//------------------Prototypes-------------------//
static NMT_result mtdr_m2c(char *motor, PCA9685_PWM_CHANNEL *channel);
static double     mtdr_get_duty_cycle(double angle);
static double     mtdr_get_angle(double duty_cycle);

NMT_result MTDR_get_current_position(char *motor, double *angle, PCA9685_settings *settings)
{
    //Input     : Name of the motor
    //Output    : Angle of the motor
    //Function  : Get the duty_cycle of PWM channel based 
    //            on motor name and calcualte angle
    
    NMT_log_write(DEBUG, "> motor=%s", motor);

    /* Initialize varibles */
    NMT_result result = OK;
    PCA9685_PWM_CHANNEL channel;

    /* Get corresponding channel to motor string */
    result = mtdr_m2c(motor, &channel);

    /* Get duty cycle */
    if (result == OK)
    {
        result = PCA9685_getPWM(settings, channel);
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

NMT_result MTDR_move_motor(char *motor, double angle, PCA9685_settings *settings)
{
    //Input     : Name of the motor and angle
    //Output    : N/A
    //Function  : Call appropriate function to move the motor to desired position 

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
        result = PCA9685_setPWM(settings, channel);
    }
    else
    {
        NMT_log_write(ERROR, "Unable to find motor=%s in list of known motors", motor);
    }

    NMT_log_write(DEBUG, "< result=%s",result_e2s[result]);
    return result;
}

NMT_result MTDR_get_pca9685_status(PCA9685_settings *settings, bool *initialized)
{
    //Input     : Name of the motor and angle
    //Output    : N/A
    //Function  : Get the status of PCA9685 (Initialized || Not Initialzied)

    /* Initialize Varibles */
    NMT_result result      = OK;

    NMT_log_write(DEBUG, "> fd=%d", settings->fd);

    /* Get the status from hardware */
    PCA9685_get_init_status(settings, initialized);

    /* Exit the function */
    NMT_log_write(DEBUG, "< result=%s", result_e2s[result]);
    return result;
}

NMT_result MTDR_init(PCA9685_settings *settings)
{
    //Input     : PCA9685_settings Structure
    //Output    : N/A
    //Function  : Initialize the PCA9685 Driver and the 
    //            Motors to Home Postion

    /* Initialize Varibles */
    NMT_result result = OK;

    settings->freq = LD27MG_FREQ;
    settings->delay_time = 0;

    /* Initialize the PCA9685 Driver */
    result = PCA9685_init(settings);

    NMT_log_write(DEBUG, "> fd=%d", settings->fd);
    return result;
}

NMT_result MTDR_seti2c(PCA9685_settings *settings)
{
    //Input     : PCA9685 settings structure
    //Output    : fd (Pointer to access IC via I2C)
    //Function  : Configure I2C Communication with PCA9685

    //Initialize Varibles
    NMT_result result  = OK;

    NMT_log_write(DEBUG, " >");

    //Initialize I2C Communication
    wiringPiSetup();
    settings->fd = wiringPiI2CSetup(I2C_ADDRESS);

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
    //Input     : Angle of rotation
    //Output    : On time in milliseconds
    //Function  : Convert the angle of rotation needed to on_time in ms

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
    //Input     : Duty Cycle
    //Output    : Angle
    //Function  : Convert duty_cycle to Angle

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
