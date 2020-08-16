/** 
 *  @file      ultransonic_test.cpp
 *  @brief     Read and display the Ultransonic Sensor Values
 *  @details   Query Ultransonic Sensor Values
 *  @author    Nitin Mohan
 *  @date      August 13, 2020
 *  @copyright 2020 NM Technologies
 */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <iostream>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_stdlib.h"
#include "RSXA.h"
#include "NMT_log.h"

/*--------------------------------------------------/
/                    Macros                         /
/--------------------------------------------------*/

/*--------------------------------------------------/
/                Structs/Classes/Enums              /
/--------------------------------------------------*/

/*--------------------------------------------------------------------/
/                             Start of Program                        /
/--------------------------------------------------------------------*/
int main()
{
    RSXA hw_settings  = {0};
    NMT_result result = OK;

    result = RSXA_init(&hw_settings);
    return 0;
}

