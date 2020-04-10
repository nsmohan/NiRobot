/** 
 *  @file      L9110_stub.h
 *  @brief     L9110 Library Stub
 *  @details   Stubs for L9110 Interfaces
 *  @author    Nitin Mohan
 *  @date      April 8, 2019
 *  @copyright 2020 - NM Technologies
 */

#ifndef CMOCK_TEST_L9110_STUB_H_
#define CMOCK_TEST_L9110_STUB_H_

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include "gmock/gmock.h"
#include "L9110.hpp" /* Library to be Stubbed */
#include "RSXA.h"

/* C++ Mocking Interface */
class L9110Mocker : public L9110
{
    public:
        L9110Mocker(RSXA_hw hw_config) : L9110 (hw_config) {};
        MOCK_METHOD2(L9110_move_motor, void(L9110_DIRECTIONS direction, int speed));
};

#endif
