#ifndef DEF_HCxSR04_Driver
#define DEF_HCxSR04_Driver

/** 
 *  @file      HCxSR04.hpp
 *  @brief     Header file for HCxSR04.cpp
 *  @details   Class Definition for HCxSR04
 *  @author    Nitin Mohan
 *  @date      August 13, 2020
 *  @copyright 2020 - NM Technologies
 *
 *  @History DD/MM/YY Author Description
 *           13/08/20 NITM   Add Doxygen and getter functions for private Variables
 */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <string>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_stdlib.h"

class HCxSR04
{
  public:
    /* Constructor */
    HCxSR04(std::string hw_name, int trigger, int echo, bool sim_mode)
        : hw_name(hw_name), trigger(trigger), echo(echo), sim_mode(sim_mode) {init_hw();}

    /* Destructor */
    ~HCxSR04() {};

    /* Prototypes */
    double distance();

    /* @var distancecm
     * Name of the Motor */
    double distancecm;

  private:
    /*@var hw_name
     *  Name of the hardware */
    std::string hw_name;

    /* @var trigger
     * trigger pin */
    int trigger;

    /* @var echo
     * echo pin */
    int echo;

    /* @var sim_mode
     *  Simulation Mode */
    bool sim_mode;
    
    /* Prototypes */
    void init_hw();
};

#endif
