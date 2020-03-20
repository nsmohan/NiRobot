/** 
 *  @file      NMT_log.h
 *  @brief     Header for NMT_log.c
 *  @details   External Interfaces for NMT_log
 *  @author    Nitin Mohan
 *  @date      Feb 7, 2019
 *  @copyright 2020 - NM Technologies
 */

#ifndef _NMT_log_
#define _NMT_log_

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <stdbool.h>

/*--------------------------------------------------/
/                   Local Imports                   / 
/--------------------------------------------------*/
#include "NMT_stdlib.h"



#ifdef __cplusplus
    extern "C" 
    {
#endif        

    /*--------------------------------------------------/
    /                   Structs & Enums                 / 
    /--------------------------------------------------*/

    /** @enum log_level
     * Logging Levels Enumeration */
    typedef enum {DEBUG, WARNING, ERROR} log_level;

    /** @struct NMT_log_settings
     *  Struct which holds log settings */
    struct NMT_log_settings 
    {
        /** @var log_level
         *  level for logging */
        log_level log_level;

        /** @var log_dir
         *  logging Directory */
        char      *log_dir;

        /** @var file_name
         *  Name of the log file */
        char      *file_name;
    };


    /** @struct log_settings
     *  log_settings global decleration */
    struct NMT_log_settings log_settings;

    /** @var log_level_e2s
     *  Convert Enum to string for log_level */
    const char* log_level_e2s[] = {"DEBUG", "WARNING", "ERROR"};

    //------------------Prototypes----------------------//
    extern void NMT_log_finish(void);
    extern NMT_result NMT_log_init_m(char *fname,           //In - Source file name
                                     char *log_dir,         //In - Directory to log in
                                     bool verbosity);       //In - Verbosity setting

    extern void NMT_log_write_m(int line_no,                //In - Line number func called
                                const char *func_name,      //In - Function this log was called from
                                log_level level,            //In - Log level setting
                                char *message,              //In - Message to log
                                ...) __attribute__ 
                                ((format (printf, 4, 5)));
#ifdef __cplusplus
    }
#endif

/*--------------------------------------------------/
/                   Macros                          / 
/--------------------------------------------------*/

/** @def NMT_log_init
 *  Macro definition. See NMT_log_init_m */
#define NMT_log_init(...)  NMT_log_init_m(__FILE__, __VA_ARGS__)

/** @def NMT_log_write
 *  Macro definition. See NMT_log_write_m */
#define NMT_log_write(...) NMT_log_write_m(__LINE__, __func__,  __VA_ARGS__) 

#endif
