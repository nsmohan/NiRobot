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



 
class sensorController{
	public:
		//! ITOR
		sensorController();
		
		//! DTOR
		~sensorController();

		//! Query
		double getDistance(US_pinout selSensor);

	private:
		std::vector< HCxSR04 > US_sensorBank;
};

/*--------------------------------------------------/
/                   Prototypes                      /
/--------------------------------------------------*/




#endif
