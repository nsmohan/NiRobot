/*PCA9685.h:    Header file for PCA9685 (PWM Controller library)

__author__      = "Nitin Mohan
__copyright__   = "Copy Right 2019. NM Technologies" */

#ifndef _PCA9685_
#define _PCA9685_

#include "NMT_stdlib.h"

//------------------Structs & Enums----------------//
typedef enum {CHANNEL_0,  CHANNEL_1,  CHANNEL_2,  CHANNEL_3,
              CHANNEL_4,  CHANNEL_5,  CHANNEL_6,  CHANNEL_7,
              CHANNEL_8,  CHANNEL_9,  CHANNEL_10, CHANNEL_11,
              CHANNEL_12, CHANNEL_13, CHANNEL_14, CHANNEL_15} pwm_channel;

typedef struct PCA9685_settings
{
    int    fd;
    float  freq;
    double duty_cycle;
    double delay_time;
}PCA9685_settings;

//------------------Prototypes----------------------//
extern NMT_result PCA9685_init(PCA9685_settings *settings);                         //In - PCA9685 struct

extern NMT_result PCA9685_chgFreq(PCA9685_settings *settings);                      //In - PCA9685 struct

extern NMT_result PCA9685_setPWM(PCA9685_settings *settings,                        //In - PCA9685 struct
                                 pwm_channel channel);                              //In - PWM Channel
#endif
