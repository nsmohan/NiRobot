/*robot_settings.c: Library to read the robot_settings configuration file

__author__      = "Nitin Mohan
__copyright__   = "Copy Right 2019. NM Technologies" */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <json-c/json.h>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "rs.h"
#include "NMT_stdlib.h"
#include "NMT_log.h"

/*--------------------------------------------------/
/                   Constants                       /
/--------------------------------------------------*/
#define RS_SETTINGS_PATH "/home/nmohan/github/NiRobot/config/rs.json"

//------------------Structs & Enums----------------//
typedef struct rs_hw
{
    char **hw_name;
    bool *hw_sim_mode;
    int  array_len;
}rs_hw;

/*------------------Prototypes----------------------*/
static NMT_result rs_parse_json(char *data_to_parse, rs_hw *hw);

NMT_result rs_get_mode(char *hw_name, bool *sim_mode)
{
    //Input     : Name of hardware to check 
    //Output    : Mode bit
    //Function  : Read rs_hw structure and set the rs_mode flag

    NMT_log_write(DEBUG, "> hw_name: %s", hw_name);

    //Initialize Variables
    NMT_result result = OK;
    rs_hw hw          = {0};
    char *file_content;

    /* Read and parse the Json settings file */
    result = NMT_stdlib_read_file(RS_SETTINGS_PATH, &file_content);

    if (result == OK)
        result = rs_parse_json(file_content, &hw);
        
    /* Search for the hardware name of interest */
    if (result == OK)
    {
        for (int i = 0; i < hw.array_len; i++)
        {
            if (!strcmp(hw.hw_name[i], hw_name))
            {
                *sim_mode = hw.hw_sim_mode[i];
                return result = OK;
            }
        }
    }
    else
    {
        NMT_log_write(ERROR, "JSON data parse error");
    }

    /* Exit the function */
    NMT_log_write(DEBUG, "< hw_name: %s sim_mode: %s result: %s",
                  hw_name, btoa(sim_mode), result_e2s[result]);
    return result = NOK;

}

static NMT_result rs_parse_json(char *data_to_parse, rs_hw *hw)
{
    //Input     : Data to be parsed and the structure to be populated with data
    //Output    : Structure with the parsed json data 
    //Function  : Parse the "hw" key in the json data

    NMT_log_write(DEBUG, "> ");

    //Initialize Variables
    NMT_result result   = OK;
    int array_len       = 0;
    int key_found       = 0;
    char *root_key      = "hw";
    char *child_keys[2] = {"hw_sim_mode", "hw_name"};
    int  no_of_keys     = sizeof(child_keys)/sizeof(child_keys[0]); 
    struct json_object *jobj;
    struct json_object *jvalue;
    struct json_object *jobj_hw;

    //Parse json file_contents
    jobj = json_tokener_parse(data_to_parse);
    json_object_object_get_ex(jobj, root_key, &jobj_hw);

    //Get Number of elements in the array
    array_len = json_object_array_length(jobj_hw);
    array_len < 1 ? (result = NOK) : (result = OK);

    NMT_log_write(DEBUG, "Array Length: %d", array_len);
    if (result == OK)
    {
        //Allocate memory based on the array length
        hw->hw_name = (char **)malloc(sizeof(char *) * array_len);
        hw->hw_sim_mode = (bool *)malloc(sizeof(bool) * array_len);

        //Save array length to structure
        hw->array_len = array_len;
       
        //Parse each element in the array and populate the structure
        for (int i = 0; i < array_len; i++)
        {
            jvalue = json_object_array_get_idx(jobj_hw, i);
            json_object_object_foreach(jvalue, key, val)
            {
                NMT_log_write(DEBUG, "key found is: %s", key);
                if (!strcmp(key, child_keys[0]))
                {
                    key_found += 1;
                    hw->hw_sim_mode[i] = json_object_get_boolean(val);
                }
                else if (!strcmp(key, child_keys[1]))
                {
                    key_found += 1;
                    hw->hw_name[i] = (char *)malloc(sizeof(char) * strlen(json_object_get_string(val)) + 1);
                    strcpy(hw->hw_name[i], json_object_get_string(val));
                }
            }
        }
        if (key_found < (array_len * no_of_keys))
        {
            result = NOK;
            NMT_log_write(ERROR, "Expecting to find keys %d but found %d", (array_len *no_of_keys), key_found);
        }
    }
    else
    {
        NMT_log_write(ERROR, "Invalid array length; array_len %d", array_len);
    }

    /* Exit the function */
    NMT_log_write(DEBUG, "< result: %s", result_e2s[result]);
    return result;
}
