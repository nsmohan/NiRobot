/** 
 *  @file      MTDR.h
 *  @brief     Header file for MTDR.c
 *  @details   External interfaces for MTDR.c
 *  @author    Nitin Mohan
 *  @date      Feb 7, 2019
 *  @copyright 2020 - NM Technologies
 */

#ifndef _MTDR_
#define _MTDR_
/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <stdlib.h>
/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_log.h"
#include "PCA9685.h"

/*--------------------------------------------------/
/                   Constants                       /
/--------------------------------------------------*/
 /**  @def CAM_HRZN_MTR
  * Horizontal motor name in MTDR context */
#define CAM_HRZN_MTR "CAM_HRZN_MTR"

/**  @def CAM_VERT_MTR
 * Vertical motor name in the MTDR context */
#define CAM_VERT_MTR "CAM_VERT_MTR"

/*--------------------------------------------------/
/                   Structures                      /
/--------------------------------------------------*/

/** @var MTDR_motors[]
 * Mapping for motors and channels*/
const struct
{
    /** @var channel PWM channmel
     *  @var motor   Name of the motor */
    PCA9685_PWM_CHANNEL channel;
    const char          *motor;
} MTDR_motors [] = 
{
    {CHANNEL_15, CAM_HRZN_MTR},
    {CHANNEL_14, CAM_VERT_MTR}
};

/** MAX_NR_OF_MOTORS no. of LD27MG connected */
const int MAX_NR_OF_MOTORS = 2;

//------------------Prototypes----------------------//
#ifdef __cplusplus
    extern "C" 
    {
        NMT_result MTDR_move_motor(char *motor, double angle,
                                   PCA9685_settings *settings,
                                   bool sim_mode);

        NMT_result MTDR_get_current_position(char *motor, double *angle,
                                             PCA9685_settings *settings, bool sim_mode);

        NMT_result MTDR_get_pca9685_status(PCA9685_settings *settings,
                                           bool *initialized,
                                           bool sim_mode);

        NMT_result MTDR_init(PCA9685_settings *settings, bool sim_mode);

        NMT_result MTDR_seti2c(PCA9685_settings *settings);
    }

#else
        NMT_result MTDR_move_motor(char *motor, double angle,
                                   PCA9685_settings *settings,
                                   bool sim_mode);

        NMT_result MTDR_get_current_position(char *motor, double *angle,
                                             PCA9685_settings *settings, bool sim_mode);

        NMT_result MTDR_get_pca9685_status(PCA9685_settings *settings,
                                           bool *initialized,
                                           bool sim_mode);

        NMT_result MTDR_init(PCA9685_settings *settings, bool sim_mode);

        NMT_result MTDR_seti2c(PCA9685_settings *settings);
#endif
#endif
