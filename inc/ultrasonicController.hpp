<<<<<<< HEAD
/**
 *  @file ultrasonicController.hpp
 *  @brief Hardware abstraction header
 *  @author Mickey T Da Silva
 *  @date 2020-02-10
 *  @copyright 2020 - NM Technologies
 */

=======
>>>>>>> 6d2f0b6a528d15a1baca09387d437afeabc6f23a

#ifndef DEF_ultraSonicController
#define DEF_ultraSonicController

<<<<<<< HEAD
=======
/*ultrasonicController.hpp: Header File for ultrasonicController.cpp

__author__      = "Mickey T Da Silva"
__copyright__   = "Copy Right 2020. NM Technologies" */

>>>>>>> 6d2f0b6a528d15a1baca09387d437afeabc6f23a
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


<<<<<<< HEAD

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
=======
class sensorController{
	public:
		//ITOR
		sensorController();
		
		//DTOR
		~sensorController();

		//Query
		double getDistance(US_pinout selSensor);

	private:
>>>>>>> 6d2f0b6a528d15a1baca09387d437afeabc6f23a
		std::vector< HCxSR04 > US_sensorBank;
};

/*--------------------------------------------------/
/                   Prototypes                      /
/--------------------------------------------------*/




#endif
