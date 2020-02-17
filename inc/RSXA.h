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
        /** @typedef struct RSXA_pins
         * Structure which holds hw interface info */
        typedef struct RSXA_pins
        {
            /** @var pin_name
             * Name of GPIO Pin */
            char pin_name[20];

            /** @var pin_no
             *  Hardware Pin No */
            int  pin_no;
        } RSXA_pins;

        /*! @typedef struct RSXA_hw
         *  @brief Structure populated by RSXA_parse_json
         */
        typedef struct RSXA_hw 
        {
            /** @var hw_name
             *  @brief Hardware Name */
            char hw_name[20];

            /** @var hw_sim_mode
             *  @brief True if in Simulation else False Name */
            bool hw_sim_mode;

            /** @var hw_interface
             *  Hardware Interface Definitions */
            RSXA_pins *hw_interface;

        }RSXA_hw;

        /** @typedef struct RSXA
         * Root struct with all Robot Settings */
        typedef struct RSXA
        {
            /** @var log_dir
             *  Default Log Directory */
            char log_dir[100];

            /** @var hw
             *  Contains all hardware settnigs */
            RSXA_hw *hw;
        }RSXA;

        /* External Interfaces Definitions */
        extern void RSXA_free_hw_struct_mem(RSXA_hw *hw);
        extern NMT_result RSXA_init(RSXA_hw *hw);
        extern NMT_result RSXA_get_mode(char *hw_name, 
                                        bool *sim_mode,
                                        RSXA_hw hw);   
    }
#else
    /** @typedef struct RSXA_pins
     * Structure which holds hw interface info */
    typedef struct RSXA_pins
    {
        /** @var pin_name
         * Name of GPIO Pin */
        char pin_name[20];

        /** @var pin_no
         *  Hardware Pin No */
        int  pin_no;
    } RSXA_pins;

    /*! @typedef struct RSXA_hw
     *  @brief Structure populated by RSXA_parse_json
     */
    typedef struct RSXA_hw 
    {
        /** @var hw_name
         *  @brief Hardware Name */
        char hw_name[20];

        /** @var hw_sim_mode
         *  @brief True if in Simulation else False Name */
        bool hw_sim_mode;

        /** @var hw_interface
         *  Hardware Interface Definitions */
        RSXA_pins *hw_interface;

    }RSXA_hw;

    /** @typedef struct RSXA
     * Root struct with all Robot Settings */
    typedef struct RSXA
    {
        /** @var log_dir
         *  Default Log Directory */
        char log_dir[100];

        /** @var hw
         *  Contains all hardware settnigs */
        RSXA_hw *hw;
    }RSXA;

    /* External Interfaces Definitions */
    extern void RSXA_free_hw_struct_mem(RSXA_hw *hw);
    extern NMT_result RSXA_init(RSXA_hw *hw);
    extern NMT_result RSXA_get_mode(char *hw_name, 
                                    bool *sim_mode,
                                    RSXA_hw hw);

#endif
#endif
