/*NMT_log.c:    Library for logging support

__author__      = "Nitin Mohan
__copyright__   = "Copy Right 2019. NM Technologies" */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <json-c/json.h>
#include <stdarg.h>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_log.h"

NMT_result NMT_log_init_m(char *fname, char *log_dir, bool verbosity)
{
    //Input     : Path to input log configuration file and verbosity
    //Output    : Result of init
    //Function  : Initialize the logger_settings struct
    
    //Initialize Variables
    NMT_result result   = OK;
    char **fname_array  = NULL;
    int  no_of_items    = 0;

    //Initialize log settings based on file
    if (result == OK)
    {
        //Split File Name string
        NMT_stdlib_split(fname, "/.", &fname_array, &no_of_items);

        log_settings.file_name = (char *)malloc(sizeof(char) * strlen(fname_array[no_of_items - 2]) + 1);


        //Populate log_settings struct from func input
        strcpy(log_settings.file_name, fname_array[no_of_items - 2]);
        log_settings.log_dir = log_dir;
        verbosity ? (log_settings.log_level = DEBUG) : (log_settings.log_level = WARNING);

        //Free allocated memory
        int i = 0;
        while (i < no_of_items)
            free(fname_array[i++]);
        free(fname_array);
    }

    //Exit function
    return result;
}

void NMT_log_write_m(int line_no, const char *func_name, log_level level, char *message, ...)
{
    //Initialize Variables
    time_t  t       = time(NULL);
    struct  tm tm   = *localtime(&t);
    va_list args;
    char    *log_to_write;
    char    time_now[100];
    char    *out_file_name;
    char    *string;

    //Get varible arguments
    va_start(args, message);
    if(0 > vasprintf(&string, message, args)) string = NULL;
    va_end(args);

    //Allocate Memory
    log_to_write = (char *)malloc(sizeof(line_no) + sizeof(time_now) + 
                                 (sizeof(char *) * (strlen(func_name) + 
                                  strlen(string) + strlen(log_level_e2s[level]) + 
                                  strlen(log_settings.file_name) + 4)));

    out_file_name = (char *)malloc(sizeof(char *) * strlen(log_settings.log_dir) + 
                                   strlen(log_settings.file_name) + 5);
    //Fill Variables with data
    sprintf(time_now, "%d-%d-%d %d:%d:%d",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                                           tm.tm_hour, tm.tm_min, tm.tm_sec);

    sprintf(log_to_write, "%s->%s->%s->%s->%d: %s",
            time_now, log_level_e2s[level], log_settings.file_name,
            func_name, line_no, string);

    sprintf(out_file_name, "%s/%s.log", log_settings.log_dir, log_settings.file_name);

    //Main part of the function
    if (level >= log_settings.log_level)
        puts(log_to_write);
    NMT_stdlib_write_file(out_file_name, log_to_write);
    
    //Free Memory
    free(string);
    free(log_to_write);
    free(out_file_name);
}

void NMT_log_finish(void)
{
    /*!
     *  @brief     Function to destroy any memory used by NMT_log
     *  @return    void
     */

    /* Free Used Memory */
    free(log_settings.file_name);
}

