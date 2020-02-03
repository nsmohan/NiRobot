
#ifndef DEF_ultraSonicController
#define DEF_ultraSonicController

/*ultrasonicController.hpp: Header File for ultrasonicController.cpp

__author__      = "Mickey T Da Silva"
__copyright__   = "Copy Right 2020. NM Technologies" */

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
		//ITOR
		sensorController();
		
		//DTOR
		~sensorController();

		//Query
		double getDistance(US_pinout selSensor);

	private:
		std::vector< HCxSR04 > US_sensorBank;
};

/*--------------------------------------------------/
/                   Prototypes                      /
/--------------------------------------------------*/




#endif
