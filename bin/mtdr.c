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
#define LOG_DIR       "/tmp"
#define MAX_ANGLE     270.00
#define MIN_ANGLE     0.00
#define LD27MG_FREQ   50.00
#define LD27MG_OFFSET 0.5
#define LD27MG_SLOPE  (1/135)

//------------------Prototypes-------------------//
static double mtdr_get_duty_cycle(double angle);

/*-------Start of Program--------------*/
int main()
{

    PCA9685_settings settings;

    NMT_log_init(LOG_DIR, true);

    settings.freq = LD27MG_FREQ;
    settings.duty_cycle = get_duty_cycle(90.00);
    settings.delay_time = 0;

    PCA9685_init(&settings);
    PCA9685_setPWM(&settings, CHANNEL_15);
    PCA9685_setPWM(&settings, CHANNEL_0);
    return 0;
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
    on_time  = (angle * LD27MG_SLOPE) + LD27MG_OFFSET;
    off_time = ((1/LD27MG_FREQ) * 1000)

    duty_cycle = ((on_time/off_time) * 100);

    //Exit the function
    NMT_log_write(DEBUG, "< on_time: %.2fms off_time: %.2fms duty_cycle: %.2f ms",on_time, off_time, duty_cycle);
    return duty_cycle;
}
