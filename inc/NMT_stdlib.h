/** 
 *  @file      NMT_stdlib.h
 *  @brief     Standard Library functions for NMT Technologies Header
 *  @details   External Interfaces for NMT_stdlib
 *  @author    Nitin Mohan
 *  @date      Feb 7, 2019
 *  @copyright 2020 - NM Technologies
 */

#ifndef _NMT_stdlib_
#define _NMT_stdlib_

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
    const char *result_e2s[] = {"OK","NOK"};                     //Convert NMT_result to string

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

#ifdef __cplusplus
    }
#endif
#endif
