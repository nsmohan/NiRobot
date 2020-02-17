/**
 *  @file ultrasonicController.hpp
 *  @brief Hardware abstraction header
 *  @author Mickey T Da Silva
 *  @date 2020-02-10
 *  @copyright 2020 - NM Technologies
 */
#ifndef DEF_ultraSonicController
#define DEF_ultraSonicController

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <string>
#include <iostream>
#include <chrono>
#include <wiringPi.h>
#include <vector>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_stdlib.h"
#include "HCxSR04.hpp"
#include "NMT_log.h"
#include "RSXA.h"

/*--------------------------------------------------/
/                   Globals                         /
/--------------------------------------------------*/

/*--------------------------------------------------/
/                   Data Definitions                 /
/--------------------------------------------------*/
typedef enum {US_echoPin_front, 
              US_echoPin_back, 
              US_echoPin_left, 
              US_echoPin_right,
              US_triggerPin} US_pinout;

/*!
 * @class sensorController
 * @brief Abstraction for HCxSR04 ultrasonic sensors
 * 
 */
class sensorController{

 
	public:        
        /*!
         * @fn sensorController( bool ) 
         * @brief Container for ultrasonic sensors
         * @param[in] sim_mode      Simulation mode for ultrasonic sensors (true == Simulated, false == real)
         * @return Constructed object
         */
		sensorController(bool sim_mode);
		
        /*!
         * @fn ~sensorController( bool ) 
         * @brief Empty destructor for ultrasonic sensor
         */
		~sensorController();

        /*!
         * @fn sensorController( bool ) 
         * @brief Container for ultrasonic sensors
         * @param[in] sim_mode      Simulation mode for ultrasonic sensors (true == Simulated, false == real)
         * @return Constructed object
         */
		double getDistance(US_pinout selSensor);

	private:
    
        /*!
         * @var US_sensorBank
         * @brief Container for ultrasonic sensors
         */

		std::vector< HCxSR04 > US_sensorBank;
};





#endif
