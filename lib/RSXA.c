/** 
 *  @file      RSXA.c
 *  @brief     Facility for reading RSXA.json
 *  @details   Read RSXA.json file and write contents to memory for 
 *             use by other components to determine their sim_mode.
 *  @author    Nitin Mohan
 *  @date      Feb 6, 2019
 *  @copyright 2020 - NM Technologies
 */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <json-c/json.h>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "RSXA.h"
#include "NMT_stdlib.h"
#include "NMT_log.h"

/*--------------------------------------------------/
/                   Macros                          /
/--------------------------------------------------*/
/** \def RS_SETTINGS_PATH 
 *  Hard coded path to the RSXA.json file
*/
#define RS_SETTINGS_PATH "/etc/NiBot/RSXA.json"

//------------------Structs & Enums----------------//

/*------------------Prototypes----------------------*/
static NMT_result RSXA_parse_json(char *data_to_parse, RSXA_hw *hw);

NMT_result RSXA_init(RSXA_hw *hw)
{
    /*!
     *  @brief     Read RSXA.json file and parse to RSXA_parse_json
     *              to parse the contents
     *  @param[in] hw
     *  @return    NMT_result
     */

    NMT_log_write(DEBUG, "> ");

    /* Initialize Variables */
    NMT_result result = OK;
    char *file_content;

    /* Read and parse the Json settings file */
    result = NMT_stdlib_read_file(RS_SETTINGS_PATH, &file_content);

    if (result == OK)
        result = RSXA_parse_json(file_content, hw);

    /* Free Used Memory */
    free(file_content);

    /* Exit the Function */
    NMT_log_write(DEBUG, "< result: %s", result_e2s[result]);
    return result;
}

NMT_result RSXA_get_mode(char *hw_name, bool *sim_mode, RSXA_hw hw)
{
    /*!
     *  @brief      Read the hw struct and determine the sim mode
     *  @param[in]  hw
     *  @param[in]  hw_name
     *  @param[out] sim_mode
     *  @return     NMT_result
     */

    NMT_log_write(DEBUG, "> hw_name: %s", hw_name);

    /* Initialize Variables */
    NMT_result result = OK;
    bool hw_name_found = false;

        
    /* Search for the hardware name of interest */
    for (int i = 0; i < hw.array_len; i++)
    {
        if (!strcmp(hw.hw_name[i], hw_name))
        {
            *sim_mode = hw.hw_sim_mode[i];
            hw_name_found = true;
            result = OK;
            break;
        }
    }

    if (!hw_name_found)
    {
        NMT_log_write(ERROR, "JSON data parse error");
        result = NOK;
    }

    /* Exit the function */
    NMT_log_write(DEBUG, "< hw_name: %s sim_mode: %s result: %s",
                  hw_name, btoa(sim_mode), result_e2s[result]);

    return result;

}

static NMT_result RSXA_parse_json(char *data_to_parse, RSXA_hw *hw)
{
    /*!
     *  @brief      Parse JSON data passed and populate the RSXA_hw struct
     *  @param[in]  data_to_parse
     *  @param[out] hw struct
     *  @return     NMT_result
     */

    NMT_log_write(DEBUG, "> ");

    /* Initialize Variables */
    NMT_result result   = OK;
    int array_len       = 0;
    int key_found       = 0;
    char *root_key      = "hw";
    char *child_keys[2] = {"hw_sim_mode", "hw_name"};
    int  no_of_keys     = sizeof(child_keys)/sizeof(child_keys[0]); 
    struct json_object *jobj;
    struct json_object *jvalue;
    struct json_object *jobj_hw;

    /* Parse json file_contents */
    jobj = json_tokener_parse(data_to_parse);
    json_object_object_get_ex(jobj, root_key, &jobj_hw);

    /* Get Number of elements in the array */
    array_len = json_object_array_length(jobj_hw);
    array_len < 1 ? (result = NOK) : (result = OK);

    NMT_log_write(DEBUG, "Array Length: %d", array_len);
    if (result == OK)
    {
        /* Allocate memory based on the array length */
        hw->hw_name = (char **)malloc(sizeof(char *) * array_len);
        hw->hw_sim_mode = (bool *)malloc(sizeof(bool) * array_len);

        /* Save array length to structure */
        hw->array_len = array_len;
       
        /* Parse each element in the array and populate the structure */
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
        NMT_log_write(ERROR, "Invalid array length (array_len=%d)", array_len);
    }

    /* Exit the function */
    NMT_log_write(DEBUG, "< result: %s", result_e2s[result]);
    return result;
}

void RSXA_free_hw_struct_mem(RSXA_hw *hw)
{
     /*!
     *  @brief      Free RSXA_hw object memory
     *  @param[in]  hw
     *  @return     void
     */
    
    NMT_log_write(DEBUG, ">");

    /* Free the Memory */
    for (int i = 0; i < hw->array_len; i++)
    {
        free(hw->hw_name[i]);
    }
    free(hw->hw_name);
    free(hw->hw_sim_mode);

    NMT_log_write(DEBUG, "<");
}
