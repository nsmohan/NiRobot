/*logger.c:    Library for logging support

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

//------------------Structures---------------------//
typedef enum
{
    OK,
    NOK
} NMT_result;

struct logger_settings {

    const char *log_level;
    const char *log_dir;
    char *out_file_name;
    bool  log_verbosity; 
};

typedef enum {DEBUG, WARNING, ERROR} log_level;

//-------------Initialize Global Variables----------//
struct logger_settings log_settings;
static const char *result_enum2str[] = {"OK","NOK"};
static const char *log_level_enum2str[] = {"DEBUG", "WARNING", "ERROR"};

//------------------Prototypes----------------------//
NMT_result logger_init_m(char *fname, char *config_file_path, bool verbosity);
NMT_result logger_log_m(const char *fname, int line_no, const char *func_name, char *message, log_level level);
static NMT_result logger_read_file(char *filepath, char *file_content);
static void logger_parse_json(char *file_content);
char split(char *string, char *parm);

#define logger_log(...) logger_log_m(__FILE__, __LINE__, __func__,  __VA_ARGS__) 
#define logger_init(...) logger_init_m(__FILE__, __VA_ARGS__) 

int main()
{
    NMT_result result = OK;
    char *file_path = "/home/nmohan/Documents/github/NiRobot/config/logger_settings.json";
    bool verbosity = false;
    
    result = logger_init(file_path, verbosity);
    result = logger_log("I am the king of the world", DEBUG);

    return 0;
}

NMT_result logger_init_m(char *fname, char *config_file_path, bool verbosity)
{
    //Input     : Path to input log configuration file and verbosity
    //Output    : log_settings structure
    //Function  : Initialize the logger_settings struct
    
    NMT_result result = OK;
    size_t fname_length = sizeof(fname);
    char file_content[1000];


    logger_read_file(config_file_path, file_content);

    if (result == OK)
    {
        logger_parse_json(file_content);
        log_settings.log_verbosity = verbosity;
    }
    else
    {
        return result;
    }
    return result;
}

NMT_result logger_log_m(const char *fname, int line_no, const char *func_name, char *message, log_level level)
{
    NMT_result result = OK;
    if (level == DEBUG)
    {
        printf("The level is debug\n");
    }
    return result;

}

static NMT_result logger_read_file(char *filepath, char *file_content)
{
    //Input     : File Path, file_content pointer
    //Output    : N/A
    //Function  : Read file and populate file_content with contents

    NMT_result result = OK;
    FILE *fp;
    int c; 
    int index = 0;
    
    if (access(filepath, F_OK) != -1)
    {
        fp = fopen(filepath, "r");
        while((c = fgetc(fp)) != EOF)
        {
            file_content[index] = c;
            index++;
        }
        file_content[index] = '\0';
        return result;
    }
    else
    {
        result = NOK;
        return result;
    }
}

char split(char *string, char *parm)
{
    char *str

    strcpy(str, string);
    item_array[i] = strtok(file_name, "/");
    while(item_array[i] != NULL)
    {
        item_array[++i]= strtok(NULL, "/");
    }

    for (int ai = 0; ai < 2; ai++)
    {
        printf("%s\n", item_array[ai]);
    }
}
static void logger_parse_json(char *file_content)
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

