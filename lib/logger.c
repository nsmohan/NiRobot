/*logger.c:    Library for logging support

__author__      = "Nitin Mohan
__copyright__   = "Copy Right 2019. NM Technologies" */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <json-c/json.h>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/

/*--------------------------------------------------/
/                   Constants                       /
/--------------------------------------------------*/

//------------------Structures---------------------//
typedef struct {

    const char *log_level;
    const char *log_dir;
    bool  log_verbosity; 

} logger_settings_struct;

//------------------Prototypes----------------------//
logger_settings_struct logger_init(char *config_file_path, bool verbosity);
static void logger_read_file(char *filepath, char *file_content);
static void logger_parse_json(char *file_content, logger_settings_struct *object_p);


int main()
{
    logger_settings_struct log_settings;
    char *file_path = "/home/nmohan/Documents/github/NiRobot/config/logger_settings.json";
    bool verbosity = true;

    log_settings = logger_init(file_path, verbosity);

    printf("log_level: %s\nlog_dir: %s\nlog_verbositiy: %d\n" , 
            log_settings.log_level, log_settings.log_dir, log_settings.log_verbosity);

    return 0;
}

logger_settings_struct logger_init(char *config_file_path, bool verbosity)
{
    //Input     : Path to input log configuration file and verbosity
    //Output    : log_settings structure
    //Function  : Initialize the logger_settings struct

    logger_settings_struct log_settings;

    char file_content[1000];
    logger_read_file(config_file_path, file_content);
    logger_parse_json(file_content, &log_settings);
    log_settings.log_verbosity = verbosity;

    return log_settings;
}

static void logger_read_file(char *filepath, char *file_content)
{
    //Input     : File Path, file_content pointer
    //Output    : N/A
    //Function  : Read file and populate file_content with contents

    FILE *fp;
    int c; 
    int index = 0;
    
    fp = fopen(filepath, "r");
    while((c = fgetc(fp)) != EOF)
    {
        file_content[index] = c;
        index++;
    }
    file_content[index] = '\0';
}

static void logger_parse_json(char *file_content, logger_settings_struct *object_p)
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
    object_p->log_level = json_object_get_string(jobj_log_level);
    object_p->log_dir   = json_object_get_string(jobj_log_dir);
}

