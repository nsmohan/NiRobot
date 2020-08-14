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
#define MAX_CHAR_LEN_SHORT 20
#define MAX_CHAR_LEN_LONG 100

/*------------------Prototypes----------------------*/
#ifdef __cplusplus
    extern "C" 
    {
#endif

            /** @var pin_no
             *  Hardware Pin No */
            int  pin_no;
        } RSXA_pins;

        /*! @struct RSXA_hw
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

            /** @var array_len_hw_int
             *  No of hw interfaces */
            int array_len_hw_int;

        }RSXA_hw;

        /** @struct RSXA
         * Root struct with all Robot Settings */
        typedef struct RSXA
        {
            /** @var log_dir
             *  Default Log Directory */
            char log_dir[100];

            /** @var hw
             *  Contains all hardware settnigs */
            RSXA_hw *hw;

            /** @var array_len_hw
             *  No of hw structs */
            int array_len_hw;
            }RSXA;

        /* External Interfaces Definitions */
        extern NMT_result RSXA_init(RSXA *RSXA_Object);
        extern void RSXA_free_mem(RSXA *RSXA_Object);
    }
#else
    /** @struct RSXA_pins
     * Structure which holds hw interface info */
    typedef struct RSXA_pins
    {
        /** @var pin_name
         * Name of GPIO Pin */
        char pin_name[MAX_CHAR_LEN_SHORT];

        /** @var pin_no
         *  Hardware Pin No */
        int  pin_no;
    } RSXA_pins;

    /*! @struct RSXA_hw
     *  @brief Structure populated by RSXA_parse_json
     */
    typedef struct RSXA_hw 
    {
        /** @var hw_name
         *  @brief Hardware Name */
        char hw_name[MAX_CHAR_LEN_SHORT];

        /** @var hw_sim_mode
         *  @brief True if in Simulation else False Name */
        bool hw_sim_mode;

        /** @var hw_interface
         *  Hardware Interface Definitions */
        RSXA_pins *hw_interface;

        /** @var array_len_hw_int
         *  No of hw interfaces */
        int array_len_hw_int;

    }RSXA_hw;

    /** @struct RSXA_procs
     *  Process Settings */
    typedef struct RSXA_procs
    {
        /** @var proc_name
         *  Process Name */
        char proc_name[MAX_CHAR_LEN_SHORT];

        /** @var server_ip
         *  IP Address that server subsribes to */
        char server_ip[MAX_CHAR_LEN_SHORT];

        /** @var server_p
         *  Server Port Number */
        int server_p;

        /** @var client_ip
         *  IP Address that client subsribes to */
        char client_ip[MAX_CHAR_LEN_SHORT];

        /** @var client_p
         *  Client Port Number */
        int client_p;

    }RSXA_procs;

    /** @struct RSXA
     * Root struct with all Robot Settings */
    typedef struct RSXA
    {
        /** @var log_dir
         *  Default Log Directory */
        char log_dir[MAX_CHAR_LEN_LONG];

        /** @var procs
         *  List of all proceses */
        RSXA_procs *procs;

        /** @var hw
         *  Contains all hardware settnigs */
        RSXA_hw *hw;

        /** @var array_len_procs
         *  No of proc structs */
        int array_len_procs;

        /** @var array_len_hw
         *  No of hw structs */
        int array_len_hw;
        }RSXA;

    /* External Interfaces Definitions */
    extern NMT_result RSXA_init(RSXA *RSXA_Object);
    extern void RSXA_free_mem(RSXA *RSXA_Object);

#ifdef __cplusplus
    }
#endif
#endif
