/** 
 *  @file      LD27MG_stub.cc
 *  @brief     LD27MG Library Stub
 *  @details   Stubs for LD27MG Interfaces
 *  @author    Nitin Mohan
 *  @date      April 8, 2019
 *  @copyright 2020 - NM Technologies
 */

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "LD27MG_stub.h"

/* Mock Interface Definitions */
CMOCK_MOCK_FUNCTION2(LD27MGMocker, LD27MG_move_motor, NMT_result(LD27MG_MOTORS, double));
CMOCK_MOCK_FUNCTION2(LD27MGMocker, LD27MG_get_current_position, NMT_result(LD27MG_MOTORS, double*));
CMOCK_MOCK_FUNCTION1(LD27MGMocker, LD27MG_init, NMT_result(RSXA_hw));
