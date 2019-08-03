/*NMT_log.h: Library of standard functions for 
 *                 NM Technologies

__author__      = "Nitin Mohan
__copyright__   = "Copy Right 2019. NM Technologies" */

#ifndef _NMT_log_
#define _NMT_log_

/*--------------------------------------------------/
/                   Local Imports                   / 
/--------------------------------------------------*/
#include "NMT_stdlib.h"

//------------------Structs & Enums----------------//
typedef enum {DEBUG, WARNING, ERROR} log_level;

struct NMT_log_settings 
{
    const char *log_level;
    const char *log_dir;
    char *out_file_name;
    bool  log_verbosity; 
};

//--------------Global Definitions----------------//
struct NMT_log_settings log_settings;
static const char *log_level_enum2str[] = {"DEBUG", "WARNING", "ERROR"};

//------------------Prototypes----------------------//
extern NMT_result NMT_log_init_m(char *fname, char *config_file_path, bool verbosity);
extern void NMT_log_write_m(const char *fname, int line_no, const char *func_name, char *message, log_level level);

//------------------------Defines----------------//
#define NMT_log_init(...)  NMT_log_init_m(__FILE__, __VA_ARGS__)
#define NMT_log_write(...) NMT_log_write_m(__FILE__, __LINE__, __func__,  __VA_ARGS__) 

#endif
