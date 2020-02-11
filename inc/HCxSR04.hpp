/** 
 *  @file       HCxSR04.hpp
 *  @brief     Header file for HCxSR04.cpp
 *  @details   External interfaces to query ultrasonic sensors
 *  @author    Nitin Mohan
 *  @date      Feb 7, 2019
 *  @copyright 2020 - NM Technologies
 */

#ifndef DEF_HCxSR04_Driver
#define DEF_HCxSR04_Driver

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <string>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_stdlib.h"

/*--------------------------------------------------/
/                   Globals                         /
/--------------------------------------------------*/
/*!
 * @class HCxSR04
 * @brief Driver for HCxSR04 Ultrasonic Sensor
 * 
 */
class HCxSR04
{
  public:
    /*!
     * @fn HCxSR04( std::string, int, bool ) 
     * @brief Constructor for Ultrasonic controller
     * @param[in] hw_name       Name of ultrasonic sensor defined in RSXA.json
     * @param[in] echo          Data return pin
     * @param[in] sim_mode      Simulate hardware with hard-coded return values 
     * @param[in] trigger       Pin which causes ultrasonic sensor to return value from echo pin
     * @return Constructed object
     */
    HCxSR04(std::string hw_name, int trigger, int echo, bool sim_mode);
    
    /*!
     * @var distanceMeters
     * @brief Distance measured by ultrasonic sensor in units of \f$ [m] \f$
     */
    double distanceMeters;
    
    /*!
     * @fn double distance()
     * @return double --> Distance measured by ultrasonic in units of \f$ [m] \f$
     */
    double distance();

  private:
    /*!
     * @fn void init_hw()
     * @brief Serially connect to ultrasonic sensor
     */
    void init_hw();
    
    /*!
     * @var int trigger
     * @brief Pin which causes sensor to take measurement and send over the "echo" pin
     */
    int trigger;
    
    /*!
     * @var int echo
     * @brief Pin which returns ultrasonic sensor data
     */
    int echo;
    
    /*!
     * @var std::string hw_name
     * @brief Ultrasonic sensor hardware name defined in JSON
     */
    std::string hw_name;
    
    /*!
     * @var bool sim_mode
     * @brief True causes HCxSR04 object to return dummy values
     */    
    bool sim_mode;
    
    /*!
     * @var volatile long startTimeUsec
     * @brief Time that trigger pin is queried for U.S. measurement
     */
    volatile long startTimeUsec;
    
    /*!
     * @var volatile long endTimeUsec
     * @brief Time at which echo pin returns measurement
     */
    volatile long endTimeUsec;
    
    /*!
     * @var long travelTimeUsec
     * @brief Time between trigger and echo for calculating distance
     */
    long travelTimeUsec;
};

#endif
