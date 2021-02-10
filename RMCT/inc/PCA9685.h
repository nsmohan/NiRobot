/** 
 *  @file      PCA9685.h
 *  @brief     Header file for PCA9685.c (PWM Controller Library)
 *  @details   Definitions of External Interfaces
 *  @author    Nitin Mohan
 *  @date      Feb 7, 2019
 *  @copyright 2020 - NM Technologies
 */

#ifndef _PCA9685_
#define _PCA9685_

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <stdbool.h>

/*--------------------------------------------------/
/                   Other Component Imports         /
/--------------------------------------------------*/
#include "NMTX/NMT_stdlib.h"

#ifdef __cplusplus
    extern "C" 
    {

#endif
    /** @def PCA9685_HW_NAME
     * Name of PCA9685 Driver */
    #define PCA9685_HW_NAME "PCA9685_PWM_DRIVER"

    /** @enum PCA9685_PWM_CHANNEL
     * Enumatation of PWM Channels */
    typedef enum {CHANNEL_0,
                  CHANNEL_1,
                  CHANNEL_2,
                  CHANNEL_3,
                  CHANNEL_4,
                  CHANNEL_5,
                  CHANNEL_6,
                  CHANNEL_7,
                  CHANNEL_8,
                  CHANNEL_9,
                  CHANNEL_10,
                  CHANNEL_11,
                  CHANNEL_12,
                  CHANNEL_13,
                  CHANNEL_14,
                  CHANNEL_15} PCA9685_PWM_CHANNEL;

    /** @typedef PCA9685_settings
     *  PCA9685 Settings Sturucture */
    typedef struct PCA9685_settings
    {
        /**@var freq
         * PCA9685 PWM frequency */
        float  freq;

        /**var sim_mode
         * Simulation Mode for PCA9685 Driver */
        bool sim_mode;

    }PCA9685_settings;

    //------------------Prototypes----------------------//
    extern NMT_result PCA9685_init(PCA9685_settings settings);

    extern NMT_result PCA9685_chgFreq(float freq);

    extern NMT_result PCA9685_setPWM(double duty_cycle, double delay_time,
                                     PCA9685_PWM_CHANNEL channel);

    extern NMT_result PCA9685_getPWM(double *duty_cycle,
                                     PCA9685_PWM_CHANNEL channel);

    extern NMT_result PCA9685_get_init_status(bool *initialized);

    extern float PCA9685_get_curret_freq();

#ifdef __cplusplus
}
#endif
#endif
