/** 
 *  @file      wiringPi_stub.h
 *  @brief     Wiring Pi Library Stub
 *  @details   Stubs for Wiring Pi Interfaces
 *  @author    Nitin Mohan
 *  @date      March 21, 2019
 *  @copyright 2020 - NM Technologies
 */

#ifndef CMOCK_TEST_WIRINGPI_STUB_H_
#define CMOCK_TEST_WIRINGPI_STUB_H_

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include "cmock/cmock.h"
#include "wiringPi.h"
#include "wiringPiI2C.h" /* Library being stubbed */

/* C++ Mocking Interface */
class wiringPiMocker : public CMockMocker<wiringPiMocker>
{
public:
    MOCK_METHOD3(wiringPiI2CWriteReg8, int(int, int, int));
    MOCK_METHOD3(wiringPiI2CWriteReg16, int(int, int, int));
    MOCK_METHOD2(wiringPiI2CReadReg8, int(int, int));
    MOCK_METHOD2(wiringPiI2CReadReg16, int(int, int));
    MOCK_METHOD1(wiringPiI2CSetup, int(int));
    MOCK_METHOD0(wiringPiSetup, int(void));
    MOCK_METHOD2(pinMode, void(int, int));
    MOCK_METHOD2(digitalWrite, void(int, int));
    MOCK_METHOD1(digitalRead, int(int));
};

#endif /* CMOCK_TEST_MATH_MOCKER_H_ */
