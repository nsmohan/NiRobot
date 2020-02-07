/*.h:         <Comment>

__author__      = "Nitin Mohan
__copyright__   = "Copy Right 2019. NM Technologies" */

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
         *  @brief -> Structure populated by RSXA_parse_json
         *  @var hw_name: Name of the hardware
         *  @var hw_sim_mode: Simulation mode of the hardware
         *  @var array_len: Length of the array item
         */
        typedef struct RSXA_hw
        {
            char **hw_name;
            bool *hw_sim_mode;
            int  array_len;
        }RSXA_hw;

        extern NMT_result RSXA_init(RSXA_hw *hw);       /** @param[in] - RSXA_hw struct */
        extern NMT_result RSXA_get_mode(char *hw_name,  /** @param[in] - Hardware Name */
                                        bool *sim_mode, /** @param[out] - True if in Simulation */
                                        RSXA_hw hw);    /** @param[in] - RSXA_hw struct */

    }
#else

    /*! @typedef struct RSXA_hw
     *  @brief -> Structure populated by RSXA_parse_json
     *  @var hw_name: Name of the hardware
     *  @var hw_sim_mode: Simulation mode of the hardware
     *  @var array_len: Length of the array item
     */
    typedef struct RSXA_hw
    {
        char **hw_name;
        bool *hw_sim_mode;
        int  array_len;
    }RSXA_hw;

    extern NMT_result RSXA_init(RSXA_hw *hw);       /** @param[in] - RSXA_hw struct */
    extern NMT_result RSXA_get_mode(char *hw_name,  /** @param[in] - Hardware Name */
                                    bool *sim_mode, /** @param[out] - True if in Simulation */
                                    RSXA_hw hw);    /** @param[in] - RSXA_hw struct */

#endif
#endif
