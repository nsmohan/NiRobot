/** 
 *  @file      ADS115.hpp
 *  @brief     Header file for ADS115.cpp 
 *  @details   Public interfaces and varibles for ADS115 
 *  @author    Nitin Mohan
 *  @date      October 11, 2020
 *  @copyright 2020
 */

#ifndef _ADS115_
#define _ADS115_

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "RSXA.h"

/*--------------------------------------------------/
/                    Macros                         /
/--------------------------------------------------*/

/*--------------------------------------------------/
/                Structs/Classes/Enums              /
/--------------------------------------------------*/
/* @class ADS115
 *  ADS115 Object */
class ADS115
{
    public:

        /* Constructor/Destructer */
        ADS115(RSXA_hw hw_config);
        ~ADS115() {};

        /* Public Methods */
        void ADS115_reset();
        double ADS115_get_voltage(std::string hw_name);

    private:

        /* @var sim_mode
         * Simulation Mode for ADS115 */
        bool sim_mode;

        /* @var hw_name
         * Device names */
        std::string hw_name;

        /* @var fd
         * File Descripter */
        int fd;

        /* Private Methods */
        void ADS115_begin_conversion_for_input(std::string hw_name);
        double ADS115_retrive_voltage_from_register();
        void ADS115_wait_for_converstion_to_complete();
};

#endif
