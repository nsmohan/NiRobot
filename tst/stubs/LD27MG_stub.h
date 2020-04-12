/** 
 *  @file      LD27MG_stub.h
 *  @brief     LD27MG Library Stub
 *  @details   Stubs for LD27MG Interfaces
 *  @author    Nitin Mohan
 *  @date      April 8, 2019
 *  @copyright 2020 - NM Technologies
 */

#ifndef CMOCK_TEST_LD27MG_STUB_H_
#define CMOCK_TEST_LD27MG_STUB_H_

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include "cmock/cmock.h"
#include "LD27MG.h" /* Library to be Stubbed */

/* C++ Mocking Interface */
class LD27MGMocker : public CMockMocker<LD27MGMocker>
{
public:
    MOCK_METHOD2(LD27MG_move_motor, NMT_result(LD27MG_MOTORS, double));
    MOCK_METHOD2(LD27MG_get_current_position, NMT_result(LD27MG_MOTORS, double*));
    MOCK_METHOD0(LD27MG_init, NMT_result(void));
};

#endif
