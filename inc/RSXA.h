/** 
 *  @file      RSXA.h
 *  @brief     Header file for RSXA.c
 *  @details   External Interfaces for RSXA
 *  @author    Nitin Mohan
 *  @date      Feb 6, 2019
 *  @copyright 2020 - NM Technologies
 */

#ifndef _RSXA_
#define _RSXA_
/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <stdbool.h>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_stdlib.h"

/*--------------------------------------------------/
/                   Constants                       /
/--------------------------------------------------*/

/*------------------Prototypes----------------------*/
#ifdef __cplusplus
    extern "C" 
    {
        /*! @typedef struct RSXA_hw
         *  @brief Structure populated by RSXA_parse_json
         */
        typedef struct RSXA_hw {

            /** @var hw_name
             *  @brief Hardware Name */
            char **hw_name;

            /** @var hw_sim_mode
             *  @brief True if in Simulation else False Name */
            bool *hw_sim_mode;

            /** @var array_len
             *  @brief Number of items */
            int  array_len;
        }RSXA_hw;

        /* External Interfaces Definitions */
        extern void RSXA_free_hw_struct_mem(RSXA_hw *hw);
        extern NMT_result RSXA_init(RSXA_hw *hw);
        extern NMT_result RSXA_get_mode(char *hw_name, 
                                        bool *sim_mode,
                                        RSXA_hw hw);   
    }
#else
    /*! @typedef struct RSXA_hw
     *  @brief Structure populated by RSXA_parse_json
     */
    typedef struct RSXA_hw {

        /** @var hw_name
         *  @brief Hardware Name */
        char **hw_name;

        /** @var hw_sim_mode
         *  @brief True if in Simulation else False Name */
        bool *hw_sim_mode;

        /** @var array_len
         *  @brief Number of items */
        int  array_len;
    }RSXA_hw;

    /* External Interfaces Definitions */
    extern void RSXA_free_hw_struct_mem(RSXA_hw *hw);
    extern NMT_result RSXA_init(RSXA_hw *hw);
    extern NMT_result RSXA_get_mode(char *hw_name, 
                                    bool *sim_mode,
                                    RSXA_hw hw);

#endif
#endif
