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

/*--------------------------------------------------/
/                   Global Variables                /
/--------------------------------------------------*/
const char *LOG_DIR     = "log_dir";
const char *HW          = "hw";
const char *HW_NAME     = "hw_name";
const char *HW_SIM_MODE = "hw_sim_mode";
const char *HW_GPIO_PIN = "hw_gpio_pin";
const char *PIN_NAME    = "pin_name";
const char *PIN_NO      = "pin_no";

/*------------------Prototypes----------------------*/
static NMT_result RSXA_parse_json(char *data_to_parse, RSXA *RSXA_Object);

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
    RSXA RSXA_Object;

    /* Read and parse the Json settings file */
    result = NMT_stdlib_read_file(RS_SETTINGS_PATH, &file_content);

    if (result == OK)
        result = RSXA_parse_json(file_content, &RSXA_Object);

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
    /*
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
    */

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

static NMT_result RSXA_parse_json(char *data_to_parse, RSXA *RSXA_Object)
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

    /* Parse json file_contents */
    struct json_object *rsxa_root_obj;
    struct  json_object *jobj_log_dir;
    struct  json_object *jobj_hw;
    struct  json_object *jobj_gpio;
    struct json_object *jobj_pins;
    struct json_object *jpinNo;
    struct json_object *jpinName;
    
    rsxa_root_obj = json_tokener_parse(data_to_parse);
    json_object_object_get_ex(rsxa_root_obj, LOG_DIR, &jobj_log_dir);
    json_object_object_get_ex(rsxa_root_obj, HW, &jobj_hw);

    int array_len_hw = json_object_array_length(jobj_hw);

    RSXA_Object->hw = (RSXA_hw *)malloc(sizeof(RSXA_hw) * array_len_hw);

    strcpy(RSXA_Object->log_dir, json_object_get_string(jobj_log_dir));

    for (int i = 0; i < array_len_hw; i++)
    {
         jobj_gpio = json_object_array_get_idx(jobj_hw, i);
         json_object_object_foreach(jobj_gpio, key, val)
         {
             printf("key=%s\n", key);
             if (!(strcmp(key, HW_NAME)))
             {
                 strcpy(RSXA_Object->hw[i].hw_name, json_object_get_string(val));
             }
             else if (!(strcmp(key, HW_SIM_MODE)))
             {

                 RSXA_Object->hw[i].hw_sim_mode = json_object_get_boolean(val);
             }
             else if (!(strcmp(key, HW_GPIO_PIN)))
             {

                int array_len_gpio = json_object_array_length(val);
                if (array_len_gpio > 0)
                {
                    RSXA_Object->hw[i].hw_interface = 
                        (RSXA_pins *)malloc(sizeof(RSXA_pins) * array_len_gpio);

                    
                    for (int j = 0; j < array_len_gpio; j++)
                    {
                        jobj_pins = json_object_array_get_idx(val, j);
                        json_object_object_get_ex(jobj_pins, PIN_NO, &jpinNo);
                        json_object_object_get_ex(jobj_pins, PIN_NAME, &jpinName);

                        strcpy(RSXA_Object->hw[i].hw_interface->pin_name, json_object_get_string(jpinName));
                        RSXA_Object->hw[i].hw_interface->pin_no = json_object_get_int(jpinNo);


                    }

                }
             }
         }
    }


    /* Free Memory */
    json_object_put(rsxa_root_obj);

    NMT_log_write(DEBUG, "< result: %s", result_e2s[result]);

    /* Exit the function */
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
    /*
    for (int i = 0; i < hw->array_len; i++)
    {
        free(hw->hw_name[i]);
    }
    free(hw->hw_name);
    free(hw->hw_sim_mode);
    */
    NMT_log_write(DEBUG, "<");
}
