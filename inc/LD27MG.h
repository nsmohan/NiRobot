/** 
 *  @file      LD27MG.h
 *  @brief     Header file for LD27MG.c
 *  @details   External interfaces for LD27MG.c
 *  @author    Nitin Mohan
 *  @date      Feb 7, 2019
 *  @copyright 2020 - NM Technologies
 */

#ifndef _LD27MG_
#define _LD27MG_
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

/*--------------------------------------------------/
/                   Structures                      /
/--------------------------------------------------*/

//------------------Prototypes----------------------//
#ifdef __cplusplus
    extern "C" 
    {
#endif
        /** enum LD27MG_MOTORS
         *  Camera Motion Control Motors */
        typedef enum {CAM_HRZN_MTR, CAM_VERT_MTR}LD27MG_MOTORS;

        /** enum LD27MG_m2s
         *  Convert LD27MG_MOTORS to string */
        const char* const LD27MG_m2s[] = {"CAM_HRZN_MTR", "CAM_VERT_MTR"};

        NMT_result LD27MG_move_motor(LD27MG_MOTORS motor, double angle,
                                   PCA9685_settings *settings,
                                   bool sim_mode);

        NMT_result LD27MG_get_current_position(LD27MG_MOTORS motor, double *angle,
                                             PCA9685_settings *settings,
                                             bool sim_mode);

        NMT_result LD27MG_init(PCA9685_settings *settings, bool sim_mode);

#ifdef __cplusplus
    }
#else

#endif
