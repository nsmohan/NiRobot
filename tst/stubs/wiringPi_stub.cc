/** 
 *  @file      wiringPi_stub.cc
 *  @brief     Wiring Pi Library Stub
 *  @details   Stubs for Wiring Pi Interfaces
 *  @author    Nitin Mohan
 *  @date      March 21, 2019
 *  @copyright 2020 - NM Technologies
 */

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "wiringPi_stub.h"
#include "wiringPiI2C.h" /* Library being stubbed */

/* Mock Function Definitions */
CMOCK_MOCK_FUNCTION3(wiringPiMocker, wiringPiI2CWriteReg8, int(int, int, int));
CMOCK_MOCK_FUNCTION3(wiringPiMocker, wiringPiI2CWriteReg16, int(int, int, int));
CMOCK_MOCK_FUNCTION2(wiringPiMocker, wiringPiI2CReadReg8, int(int, int));
CMOCK_MOCK_FUNCTION2(wiringPiMocker, wiringPiI2CReadReg16, int(int, int));
