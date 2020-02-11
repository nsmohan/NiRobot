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

		//! Query sensor for distance measurement
		double getDistance(US_pinout selSensor);

	private:
		//! Container for driver interfaces with ultrasonic sensors
		std::vector< HCxSR04 > US_sensorBank;
};

/*--------------------------------------------------/
/                   Prototypes                      /
/--------------------------------------------------*/




#endif
