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

/*--------------------------------------------------/
/                   Macros                          /
/--------------------------------------------------*/
/** \def RS_SETTINGS_PATH 
 *  RSXA Settings Path
*/
#define RS_SETTINGS_PATH getenv("RSXA_SETTINGS")

/** \def FILL_STRUCT 
 *  Fill RSXA Structure
*/
#define FILL_STRUCT(key_mapping, json_obj) \
    result = cp_json2struct(key_mapping, json_obj, \
                            (sizeof(key_mapping)/sizeof(key_mapping[0]))); \
/*--------------------------------------------------/
/                   Global Variables                /
/--------------------------------------------------*/
struct RSXA_STRUCT_MAPPING
{
    char *key;
    union
    {
        int    *integer;
        bool   *boolean;
        char   *character;
        double *dbl;
    };
    enum  json_type  js_type; 
};

struct RSXA_MAPPING
{
    char *key;
    union
    {
        RSXA_general_settings *general_settings;
    };
    struct RSXA_STRUCT_MAPPING *mapping;
    size_t mapping_len;
};

/*------------------Prototypes----------------------*/
static NMT_result RSXA_parse_json(char *data_to_parse, RSXA *RSXA_Object);
static NMT_result RSXA_find_key(json_object *in_obj, const char *key, json_object **out_obj);
static NMT_result fill_rsxa_struct(struct json_object *parent_obj, struct RSXA_MAPPING mapping[]);
static NMT_result cp_json2struct(struct RSXA_STRUCT_MAPPING *key_mapping,
                                 struct json_object *obj,
                                 size_t nrof_elements);
static NMT_result fill_rsxa_hw(RSXA *RSXA_Object, json_object *jobj_hw);
static NMT_result fill_rsxa_procs(RSXA *RSXA_Object, json_object *root_obj);
static NMT_result fill_general_settings(RSXA_general_settings *general_settings, json_object *root_obj);
/*--------------------------------------------------/
/                   Public Interfaces               /
/--------------------------------------------------*/
NMT_result RSXA_init(RSXA *RSXA_Object)
{
    /*!
     *  @brief     Read RSXA.json file and parse to RSXA_parse_json
     *              to parse the contents
     *  @param[in] hw
     *  @return    NMT_result
     */

    printf("Initializing RSXA ..... \n");

    /* Initialize Variables */
    NMT_result result = OK;
    char *file_content;

    /* Read and parse the Json settings file */
    result = NMT_stdlib_read_file(RS_SETTINGS_PATH, &file_content);

    if (result == OK)
    {
        result = RSXA_parse_json(file_content, RSXA_Object);

        /* Free Used Memory */
        free(file_content);

        /* Exit the Function */
        printf("Parsed: %s and the result=%s \n", RS_SETTINGS_PATH, 
                                                  result_e2s[result]);
    }
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

    /* Initialize Variables */
    NMT_result result      = OK;

    /* Create json-c objects that will be needed */
    struct json_object *rsxa_root_obj = {0};
    
    /* Parse the file */
    rsxa_root_obj = json_tokener_parse(data_to_parse);

    /* Members of Root */
    struct RSXA_STRUCT_MAPPING root_mapping[] = 
    {
        {"log_dir", .character = RSXA_Object->log_dir, json_type_string}
    };

    /* Fill Root */
    FILL_STRUCT(root_mapping, rsxa_root_obj);

    /* Fill Other Members */
    if (result == OK) {result = fill_general_settings(&(RSXA_Object->general_settings), rsxa_root_obj);}
    if (result == OK) {result = fill_rsxa_hw(RSXA_Object, rsxa_root_obj);}
    if (result == OK) {result = fill_rsxa_procs(RSXA_Object, rsxa_root_obj);}

    /* Free Memory */
    json_object_put(rsxa_root_obj);

    /* Exit the function */
    return result;
}

static NMT_result RSXA_find_key(json_object *in_obj, const char *key, json_object **out_obj)
{
    /*!
     *  @brief      Wrapper function for json_object_object_get_ex
     *  @param[in]  in_obj
     *  @param[in]  key
     *  @param[out] out_obj
     *  @return     NMT_result
     */

    /* Initialize Variables */
    NMT_result result = OK;

    if (!(json_object_object_get_ex(in_obj, key, &(*out_obj))))
    {
        result = NOK;
        printf("Parse Error! %s not found in %s \n", key, RS_SETTINGS_PATH);
    }

    /* Exit the function */
    return result;
}
static NMT_result fill_rsxa_struct(struct json_object *parent_obj, struct RSXA_MAPPING mapping_root[])
{
    NMT_result result = OK;

    struct json_object *jobj_settings = {0};
    
    /* Get the General Settings Object */
    result = RSXA_find_key(parent_obj, mapping_root->key, &jobj_settings);

    if (result == OK)
    {
        result = cp_json2struct(mapping_root->mapping, jobj_settings, mapping_root->mapping_len);
    }

    return result;
}

static NMT_result cp_json2struct(struct RSXA_STRUCT_MAPPING *key_mapping, struct json_object *obj, size_t nrof_elements)
{
    NMT_result result = OK;
    struct json_object *jvalues = {0};

    for (unsigned int i = 0; i < nrof_elements; i++)
    {
        result = RSXA_find_key(obj, key_mapping[i].key, &jvalues);

        if (result == OK)
        {
            int value;
            bool boolean;
            double dbl;
            switch (key_mapping[i].js_type)
            {

                case json_type_int:
                    value = json_object_get_int(jvalues);
                    memcpy(key_mapping[i].integer, &value, sizeof(value));
                    break;
                case json_type_string:
                    strcpy(key_mapping[i].character, json_object_get_string(jvalues));
                    break;
                case json_type_boolean:
                    boolean = json_object_get_boolean(jvalues);
                    memcpy(key_mapping[i].boolean, &boolean, sizeof(boolean));
                    break;
                case json_type_double:
                    dbl = json_object_get_int(jvalues);
                    memcpy(key_mapping[i].dbl, &dbl, sizeof(dbl));
                    break;
                case json_type_array:
                case json_type_null:
                case json_type_object:
                    printf("ERROR! Invalid json_type to parse!");
                    break;
            }
        }
    }

    return result;

}

static NMT_result fill_general_settings(RSXA_general_settings *general_settings, json_object *root_obj)
{
    /* Initialize Variables */
    NMT_result result = OK;

    struct RSXA_STRUCT_MAPPING general_settings_mapping[] = 
    {
        {"cam_mtr_step_size",   .integer = &(general_settings->cam_mtr_step_size),          json_type_int},
        {"default_drive_speed", .integer = (int *)&(general_settings->default_drive_speed), json_type_int},
        {"rsda_broadcast_freq", .integer = &(general_settings->rsda_broadcast_rate),        json_type_int}
    };

    struct RSXA_MAPPING general_settings_struct = 
    {
        "general_settings",
        {general_settings},
        general_settings_mapping,
        sizeof(general_settings_mapping)/sizeof(general_settings_mapping[0])
    };

    /* Fill Structure */
    result = fill_rsxa_struct(root_obj, &general_settings_struct);

    /* Exit Function */
    return result;
}



static NMT_result fill_rsxa_hw(RSXA *RSXA_Object, json_object *root_obj)
{
    /*!
     *  @brief     Fill RSXA_hw Structure
     *  @param[in] RSXA_Object, jobj_hw
     *  @return    OK/!NOK
     */

    /* Initialize Variables */
    NMT_result result = OK;
    struct json_object *jobj_hw = {0};
    struct json_object *jobj_hw_v = {0};
    struct json_object *jobj_hw_gpio = {0};

    /* Get the hw object */
    if (result == OK) {result = RSXA_find_key(root_obj, "hw", &jobj_hw);}

    /* Get number of hw elements */
    if (result == OK) {RSXA_Object->array_len_hw = json_object_array_length(jobj_hw);}

    /* Fill HW */
    if ((result == OK) && (RSXA_Object->array_len_hw > 0))
    {
        /* Allocate Memory for RSXA_hw */
        RSXA_Object->hw = (RSXA_hw *)malloc(sizeof(RSXA_hw) * RSXA_Object->array_len_hw);

        for (int i = 0; i < RSXA_Object->array_len_hw; i++)
        {
            jobj_hw_v = json_object_array_get_idx(jobj_hw, i);

            RSXA_hw *hw = &(RSXA_Object->hw[i]);

            struct RSXA_STRUCT_MAPPING hw_mapping[] =
            {
                {"hw_name",     .character = hw->hw_name,        json_type_string},
                {"hw_sim_mode", .boolean   = &(hw->hw_sim_mode), json_type_boolean}
            };
            FILL_STRUCT(hw_mapping, jobj_hw_v);

            /* Get hw_gpio elements */
            if (result == OK) {result = RSXA_find_key(jobj_hw_v, "hw_interface", &jobj_hw_gpio);}

            /* Get number of hw_gpio elements */
            if (result == OK) {RSXA_Object->hw[i].array_len_hw_int = json_object_array_length(jobj_hw_gpio);}

            /* Proceed if array_len_hw_int has elements */
            if ((result == OK) && (RSXA_Object->hw[i].array_len_hw_int > 0))
            {
                /* Allocate Memory for RSXA_pins struct */
                RSXA_Object->hw[i].hw_interface = 
                    (RSXA_pins *)malloc(sizeof(RSXA_pins) * RSXA_Object->hw[i].array_len_hw_int);

                for (int j = 0; j < RSXA_Object->hw[i].array_len_hw_int; j++)
                {
                    struct json_object *jobj_hw_gpio_v = {0};
                    jobj_hw_gpio_v = json_object_array_get_idx(jobj_hw_gpio, j);
                    RSXA_pins *pins = &(RSXA_Object->hw[i].hw_interface[j]);

                    struct RSXA_STRUCT_MAPPING pin_mapping[] = 
                    {
                        {"pin_name", .character = pins->pin_name,  json_type_string},
                        {"pin_no",   .integer   = &(pins->pin_no), json_type_int}
                    };

                    FILL_STRUCT(pin_mapping, jobj_hw_gpio_v);
                   }
                }
             }
    }


    /* Exit the Function */
    return result;
}

static NMT_result fill_rsxa_procs(RSXA *RSXA_Object, struct json_object *rsxa_root_obj)
{
    /*!
     *  @brief     Fill RSXA Procs Structure
     *  @param[in] RSXA_Object, jobj_procs
     *  @return    OK/!OK
     */

    /* Initialize Variables */
    NMT_result result = OK;
    struct json_object *jobj_procs_v = {0};
    struct json_object *jobj_procs = {0};

    /* Get the procs object */
    if (result == OK) {result = RSXA_find_key(rsxa_root_obj, "procs", &jobj_procs);}

    /* Get number of procs elements */
    if (result == OK) {RSXA_Object->array_len_procs = json_object_array_length(jobj_procs);}

    /* Fill Procs */
    if ((result == OK) && (RSXA_Object->array_len_procs > 0))
    {
        /* Allocate Memory for RSXA_procs */
        RSXA_Object->procs = (RSXA_procs *)malloc(sizeof(RSXA_procs) * RSXA_Object->array_len_procs);

        for (int i = 0; i < RSXA_Object->array_len_procs; i++)
        {

            RSXA_procs *procs = &(RSXA_Object->procs[i]); 
            struct RSXA_STRUCT_MAPPING procs_mapping[] = 
            {
                {"proc_name", .character = procs->proc_name,   json_type_string},
                {"server_ip", .character = procs->server_ip,   json_type_string},
                {"server_p",  .integer   = &(procs->server_p), json_type_int},
                {"client_ip", .character = procs->client_ip,   json_type_string},
                {"client_p",  .integer   = &(procs->client_p), json_type_int}
            };

            jobj_procs_v = json_object_array_get_idx(jobj_procs, i);

            FILL_STRUCT(procs_mapping, jobj_procs_v);
        }
    }

    /* Exit the Function */
    return result;
}

void RSXA_free_mem(RSXA *RSXA_Object)
{
     /*!
     *  @brief      Free RSXA_Object
     *  @param[in]  RSXA_Object
     *  @return     void
     */

    /* Free RSXA_hw */
    for (int i = 0; i < RSXA_Object->array_len_hw; i++)
    {
        if (RSXA_Object->hw[i].array_len_hw_int > 0)
            free(RSXA_Object->hw[i].hw_interface);
    }

    free(RSXA_Object->hw);

    /* Free RSXA_Procs */
    free(RSXA_Object->procs);
}
