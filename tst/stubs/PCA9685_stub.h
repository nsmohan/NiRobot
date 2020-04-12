/** 
 *  @file      PCA9685_stub.h
 *  @brief     PCA9685 Library Stub
 *  @details   Stubs for PCA9685 Interfaces
 *  @author    Nitin Mohan
 *  @date      March 22, 2019
 *  @copyright 2020 - NM Technologies
 */

#ifndef CMOCK_TEST_PCA9685_STUB_H_
#define CMOCK_TEST_PCA9685_STUB_H_

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include "cmock/cmock.h"
#include "PCA9685.h" /* Library to be Stubbed */

/* C++ Mocking Interface */
class PCA9685Mocker : public CMockMocker<PCA9685Mocker>
{
public:
    MOCK_METHOD1(PCA9685_init, NMT_result(PCA9685_settings));
    MOCK_METHOD1(PCA9685_chgFreq, NMT_result(float));
    MOCK_METHOD3(PCA9685_setPWM, NMT_result(double, double, PCA9685_PWM_CHANNEL));
    MOCK_METHOD2(PCA9685_getPWM, NMT_result(double *, PCA9685_PWM_CHANNEL));
    MOCK_METHOD1(PCA9685_get_init_status, NMT_result(bool*));
    MOCK_METHOD0(PCA9685_get_curret_freq, float()); };

#endif
