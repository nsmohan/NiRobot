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
const char *HW_GPIO_PIN = "hw_interface";
const char *PIN_NAME    = "pin_name";
const char *PIN_NO      = "pin_no";

/*------------------Prototypes----------------------*/
static NMT_result RSXA_parse_json(char *data_to_parse, RSXA *RSXA_Object);

NMT_result RSXA_init(RSXA *RSXA_Object)
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
        result = RSXA_parse_json(file_content, RSXA_Object);

    /* Free Used Memory */
    free(file_content);

    /* Exit the Function */
    NMT_log_write(DEBUG, "< result: %s", result_e2s[result]);
    return result;
}

static NMT_result RSXA_parse_json(char *data_to_parse, RSXA *RSXA_Object)
{
    /*!
     *  @brief      Parse JSON data passed and populate the RSXA Structure
     *  @param[in]  data_to_parse
     *  @param[out] RSXA_Object
     *  @return     NMT_result
     */

    NMT_log_write(DEBUG, "> ");

    /* Initialize Variables */
    NMT_result result      = OK;

    /* Create json-c objects that will be needed */
    struct json_object *rsxa_root_obj;
    struct json_object *jobj_hw;
    struct json_object *jobj_hw_v;
    struct json_object *jobj_hw_gpio;
    struct json_object *jobj_hw_gpio_v;
    struct json_object *jvalues;
    
    /* Parse the file */
    rsxa_root_obj = json_tokener_parse(data_to_parse);

    /* Get the logger directory */
    if (!(json_object_object_get_ex(rsxa_root_obj, LOG_DIR, &jvalues)))
    {
        result = NOK;
        NMT_log_write(ERROR, "Parse Error! %s not found in %s", LOG_DIR, RS_SETTINGS_PATH);
    }
            
    if (result == OK)
    {
        /* Copy log directory to struct */
        strcpy(RSXA_Object->log_dir, json_object_get_string(jvalues));

        /* Get the hw object */
        if (!(json_object_object_get_ex(rsxa_root_obj, HW, &jobj_hw)))
        {
            result = NOK;
            NMT_log_write(ERROR, "Parse Error! %s not found in %s", HW, RS_SETTINGS_PATH);
        }
    }

    if (result == OK)
    {
        /* Get number of hw elements */
        RSXA_Object->array_len_hw = json_object_array_length(jobj_hw);

        if (RSXA_Object->array_len_hw < 1)
            result = NOK;
    }

    if (result == OK)
    {
        /* Allocate Memory for RSXA_hw */
        RSXA_Object->hw = (RSXA_hw *)malloc(sizeof(RSXA_hw) * RSXA_Object->array_len_hw);

        for (int i = 0; i < RSXA_Object->array_len_hw; i++)
        {
            jobj_hw_v = json_object_array_get_idx(jobj_hw, i);

            /* Get and populate hw_name */
            if (!(json_object_object_get_ex(jobj_hw_v, HW_NAME, &jvalues)))
            {
                result = NOK;
                NMT_log_write(ERROR, "Parse Error! %s not found in %s", HW_NAME, RS_SETTINGS_PATH);
            }

            if (result == OK)
            {
                /* Copy hardware name to struct */
                strcpy(RSXA_Object->hw[i].hw_name, json_object_get_string(jvalues));

                /* Get and populate hw_sim_mode */
                if (!(json_object_object_get_ex(jobj_hw_v, HW_SIM_MODE, &jvalues)))
                {
                    result = NOK;
                    NMT_log_write(ERROR, "Parse Error! %s not found in %s", HW_SIM_MODE, RS_SETTINGS_PATH);
                }

                if (result == OK)
                {   
                    /* Copy the sim_mode to struct */
                    RSXA_Object->hw[i].hw_sim_mode = json_object_get_boolean(jvalues);

                    /* Get hw_gpio elements */
                    if (!(json_object_object_get_ex(jobj_hw_v, HW_GPIO_PIN, &jobj_hw_gpio)))
                    {
                        result = NOK;
                        NMT_log_write(ERROR, "Parse Error! %s not found in %s", HW_GPIO_PIN, RS_SETTINGS_PATH);
                    }
                    else
                    {
                        /* Get number of hw_gpio elements */
                        RSXA_Object->hw[i].array_len_hw_int = json_object_array_length(jobj_hw_gpio);
                    }
                }
             }

            if ((result == OK) && (RSXA_Object->hw[i].array_len_hw_int > 0))
            {
                /* Allocate Memory for RSXA_pins struct */
                RSXA_Object->hw[i].hw_interface = 
                    (RSXA_pins *)malloc(sizeof(RSXA_pins) * RSXA_Object->hw[i].array_len_hw_int);

                for (int j = 0; j < RSXA_Object->hw[i].array_len_hw_int; j++)
                {
                    jobj_hw_gpio_v = json_object_array_get_idx(jobj_hw_gpio, j);

                    /* Get the pin number */
                    if (!(json_object_object_get_ex(jobj_hw_gpio_v, PIN_NO, &jvalues)))
                    {
                        result = NOK;
                        NMT_log_write(ERROR, "Parse Error! %s not found in %s", PIN_NO, RS_SETTINGS_PATH);
                    }

                    if (result == OK)
                    {
                        RSXA_Object->hw[i].hw_interface[j].pin_no = json_object_get_int(jvalues);

                        if (json_object_object_get_ex(jobj_hw_gpio_v, PIN_NAME, &jvalues))
                        {
                            /* Get the pin name */
                            strcpy(RSXA_Object->hw[i].hw_interface[j].pin_name, json_object_get_string(jvalues));
                        }
                        else
                        {
                            result = NOK;
                            NMT_log_write(ERROR, "Parse Error! %s not found in %s", PIN_NAME, RS_SETTINGS_PATH);
                        }
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

void RSXA_free_mem(RSXA *RSXA_Object)
{
     /*!
     *  @brief      Free RSXA_Object
     *  @param[in]  RSXA_Object
     *  @return     void
     */

    for (int i = 0; i < RSXA_Object->array_len_hw; i++)
    {
        if (RSXA_Object->hw[i].array_len_hw_int > 0)
            free(RSXA_Object->hw[i].hw_interface);
    }

    free(RSXA_Object->hw);
}
