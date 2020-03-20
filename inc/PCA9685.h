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
#include "NMT_stdlib.h"

#ifdef __cplusplus
    extern "C" 
    {
#endif
    /** @def PCA9685_HW_NAME
     * Name of PCA9685 Driver */
    #define PCA9685_HW_NAME "PCA9685_PWM_DRIVER"

    /**@def PCA9685_I2C_ADDRESS
     * PCA9685 I2C Address */
    #define PCA9685_I2C_ADDRESS 0x40

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

    /** @var PCA9685_PWM_CHANNEL_e2s
     * Convert Enum to string var */
    const char* PCA9685_PWM_CHANNEL_e2s[] = {"CHANNEL_0",
                                             "CHANNEL_1",
                                             "CHANNEL_2",
                                             "CHANNEL_3",
                                             "CHANNEL_4",
                                             "CHANNEL_5",
                                             "CHANNEL_6",
                                             "CHANNEL_7",
                                             "CHANNEL_8",
                                             "CHANNEL_9",
                                             "CHANNEL_10",
                                             "CHANNEL_11",
                                             "CHANNEL_12",
                                             "CHANNEL_13",
                                             "CHANNEL_14",
                                             "CHANNEL_15"};
    /** @typedef PCA9685_settings
     *  PCA9685 Settings Sturucture */
    typedef struct PCA9685_settings
    {
        /** @var fd
         * WiringPi File Data pointer */
        int    fd;

        /**@var freq
         * PCA9685 PWM frequency */
        float  freq;

        /**@var duty_cycle
         * PCA9685 duty cycle */
        double duty_cycle;

        /**@var delay_time
         * Amount of delay needed in (ms) prior to starting PWM */
        double delay_time;

    }PCA9685_settings;

    //------------------Prototypes----------------------//
    extern NMT_result PCA9685_init(PCA9685_settings *settings,
                                   bool sim_mode);

    extern NMT_result PCA9685_chgFreq(PCA9685_settings *settings,
                                      bool sim_mode);

    extern NMT_result PCA9685_setPWM(PCA9685_settings *settings,
                                     PCA9685_PWM_CHANNEL channel,
                                     bool sim_mode);

    extern NMT_result PCA9685_getPWM(PCA9685_settings *settings,
                                     PCA9685_PWM_CHANNEL channel,
                                     bool sim_mode);

    extern NMT_result PCA9685_get_init_status(PCA9685_settings *settings,
                                              bool *initialized,
                                              bool sim_mode);

#ifdef __cplusplus
}
#endif
#endif
