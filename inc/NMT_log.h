/*NMT_log.h: Library of standard functions for 
 *                 NM Technologies

__author__      = "Nitin Mohan
__copyright__   = "Copy Right 2019. NM Technologies" */

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

//------------------Structs & Enums----------------//
typedef enum {DEBUG, WARNING, ERROR} log_level;


//------------------Prototypes----------------------//
#ifdef __cplusplus
    extern "C" 
    {
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
        struct NMT_log_settings 
        {
            log_level log_level;
            char      *log_dir;
            char      *file_name;
        };

        //--------------Global Definitions----------------//
        struct NMT_log_settings log_settings;
        const char* log_level_e2s[] = {"DEBUG", "WARNING", "ERROR"};
    }
#else
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
    struct NMT_log_settings 
    {
        log_level log_level;
        char      *log_dir;
        char      *file_name;
    };

    //--------------Global Definitions----------------//
    struct NMT_log_settings log_settings;
    const char* log_level_e2s[] = {"DEBUG", "WARNING", "ERROR"};

#endif

//--------------------Macros---------------------//
#define NMT_log_init(...)  NMT_log_init_m(__FILE__, __VA_ARGS__)
#define NMT_log_write(...) NMT_log_write_m(__LINE__, __func__,  __VA_ARGS__) 

#endif
