/*mtdr.c:       Motor Driver

__author__      = "Nitin Mohan
__copyright__   = "Copy Right 2019. NM Technologies" */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_log.h"
#include "PCA9685.h"

//---------------------Macros----------------------//
#define LOG_DIR    "/tmp"
#define MAX_ANGLE  180.00
#define MIN_ANGLE  0.00
#define MG995_FREQ 50.00

//------------------Prototypes-------------------//
double calc_on_time(double angle);

/*-------Start of Program--------------*/
int main()
{

    PCA9685_settings settings;

    NMT_log_init(LOG_DIR, true);

    settings.freq = 50;
    settings.duty_cycle = 12.5;
    settings.delay_time = 0;

    PCA9685_init(&settings);
    PCA9685_setPWM(&settings, CHANNEL_15);
    PCA9685_setPWM(&settings, CHANNEL_0);
    return 0;
}

double calc_on_time(double angle)
{
    //Input     : Angle of rotation
    //Output    : On time in milliseconds
    //Function  : Convert the angle of rotation needed to on_time in ms

    NMT_log_write(DEBUG, "> angle: %f",angle);
            
    //Initialize Varibles
    double on_time;

    //Ensure the provided angle is within limits
    angle = (angle > MAX_ANGLE ? MAX_ANGLE : (angle < MIN_ANGLE ? MIN_ANGLE : angle));

    //on_time (ms) =  (angle (degrees)/9) + 0.5
    on_time = (angle/9) + 0.5;

    //Exit the function
    NMT_log_write(DEBUG, "< on_time: %f",on_time);
    return on_time;
}
