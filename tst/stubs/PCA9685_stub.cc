/** 
 *  @file      PCA9685_stub.cc
 *  @brief     Stub for PCA9685 Library
 *  @details   Stubs for PCA9685 Interfaces
 *  @author    Nitin Mohan
 *  @date      March 21, 2019
 *  @copyright 2020 - NM Technologies
 */

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "PCA9685_stub.h"

/* Mock Interface Definitions */
CMOCK_MOCK_FUNCTION1(PCA9685Mocker, PCA9685_init, NMT_result(PCA9685_settings));
CMOCK_MOCK_FUNCTION1(PCA9685Mocker, PCA9685_chgFreq, NMT_result(float));
CMOCK_MOCK_FUNCTION3(PCA9685Mocker, PCA9685_setPWM, NMT_result(double, double, PCA9685_PWM_CHANNEL));
CMOCK_MOCK_FUNCTION2(PCA9685Mocker, PCA9685_getPWM, NMT_result(double *, PCA9685_PWM_CHANNEL));
CMOCK_MOCK_FUNCTION1(PCA9685Mocker, PCA9685_get_init_status, NMT_result(bool*));
CMOCK_MOCK_FUNCTION0(PCA9685Mocker, PCA9685_get_curret_freq, float());
