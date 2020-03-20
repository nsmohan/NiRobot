#ifndef CMOCK_TEST_WIRINGPI_STUB_H_
#define CMOCK_TEST_WIRINGPI_STUB_H_

#include "cmock/cmock.h"

#include "wiringPiI2C.h"

class wiringPiMocker : public CMockMocker<wiringPiMocker>
{
public:
    MOCK_METHOD3(wiringPiI2CWriteReg8, int(int, int, int));
};

#endif /* CMOCK_TEST_MATH_MOCKER_H_ */
