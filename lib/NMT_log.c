/*NMT_log.c:    Library for logging support

__author__      = "Nitin Mohan
__copyright__   = "Copy Right 2019. NM Technologies" */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <json-c/json.h>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_log.h"

//------------------Prototypes----------------------//
static void NMT_log_parse_json(char *file_content);

NMT_result NMT_log_init_m(char *fname, char *config_file_path, bool verbosity)
{
    //Input     : Path to input log configuration file and verbosity
    //Output    : Result of init
    //Function  : Initialize the logger_settings struct
    
    //Initialize Variables
    NMT_result result   = OK;
    char *file_content  = NULL;
    char **fname_array  = NULL;
    int  no_of_items    = 0;

    //Read Configuration File
    result = NMT_stdlib_read_file(config_file_path, &file_content);

    //Initialize log settings based on file
    if (result == OK)
    {
        //Split File Name string
        NMT_stdlib_split(fname, "/", &fname_array, &no_of_items);

        //Populate log_settings struct from settings file
        NMT_log_parse_json(file_content);

        //Populate log_settings struct from func input
        log_settings.log_verbosity = verbosity;
        log_settings.out_file_name = fname_array[1];

        //Free allocated memory
        free(file_content);
        free(fname_array);
    }

    //Exit function
    return result;
}

void NMT_log_write_m(const char *fname, int line_no, const char *func_name, char *message, log_level level)
{
    if (level == DEBUG)
    {
        printf("The level is debug\n");
    }
}

static void NMT_log_parse_json(char *file_content)
{
    //Input     : file_content and structure pointer
    //Output    : N/A
    //Function  : populate network_settings struct with network_settings content

    //Initialize 
    struct json_object *parsed_json;
    struct json_object *jobj_log_level;
    struct json_object *jobj_log_dir;

    //Parse json file_contents
    parsed_json = json_tokener_parse(file_content);

    //Get contents by json key
    json_object_object_get_ex(parsed_json, "log_level", &jobj_log_level);
    json_object_object_get_ex(parsed_json, "log_dir",     &jobj_log_dir);

    //Save contents to the structure
    log_settings.log_level = json_object_get_string(jobj_log_level);
    log_settings.log_dir   = json_object_get_string(jobj_log_dir);
}

