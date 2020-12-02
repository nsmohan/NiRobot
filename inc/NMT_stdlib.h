/** 
 *  @file      NMT_stdlib.h
 *  @brief     Standard Library functions for NMT Technologies Header
 *  @details   External Interfaces for NMT_stdlib
 *  @author    Nitin Mohan
 *  @date      Feb 7, 2019
 *  @copyright 2020 - NM Technologies
 *
 *  @History DD/MM/YY Author Description
 *           14/10/20 NITM   Add Function SwapBytes
 */

#ifndef _NMT_stdlib_
#define _NMT_stdlib_

#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

#ifndef __USE_XOPEN
#define __USE_XOPEN
#endif


/* --- Macros ----*/
#define btoa(x) ((x)?"true":"false")

#ifdef __cplusplus
    extern "C" 
    {
#endif

    //------------------Structs & Enums----------------//

    /** enum NMT_result
     *  OK/NOK enumeration */
    typedef enum {OK, NOK} NMT_result;                           //Standard return of function

    //--------------Global Definitions----------------//
    /** enum result_e2s
     *  OK/NOK enum_to_string */
    const char* const result_e2s[] = {"OK","NOK"};               //Convert NMT_result to string

    //------------------Prototypes----------------------//
    extern void       NMT_stdlib_split(char *string,             //In  - String to Split
                                       char *param,              //In  - deliminator(s)
                                       char ***item_array,       //Out - Array of split strings
                                       int *no_of_items);        //Out - Number of elements in array

    extern int        NMT_stdlib_count(char *string,             //In  - String to count
                                       char *param);             //In  - deliminator(s) to look for
                                                                 //Out - # of occurences of param

    extern size_t     NMT_stdlib_get_file_size(char *filepath);  //In  - Path to file
                                                                 //Out - Size of file

    extern NMT_result NMT_stdlib_read_file(char *filepath,       //In  - Path to file
                                           char **file_content); //Out - Content of file

    extern void NMT_stdlib_write_file(char *filepath,            //In  - Path to file
                                      char *file_content);       //In  - Content to write

    extern int16_t NMT_stdlib_swapBytes(int16_t Bytes);                  //In   - Bytes

    extern NMT_result NMT_stdlib_timer_interrupt(void(*funcPtrcb)(),     //In Function to be called
                                                 useconds_t interval,    //in Interval
                                                 bool continous);        //In (True = loop, false = run once

#ifdef __cplusplus
    }
#endif
#endif
