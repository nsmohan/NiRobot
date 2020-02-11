/**
 *  @file ultrasonicController.cpp
 *  @brief Hardware abstraction for ultrasonic sensors
 *  @author Mickey T Da Silva
 *  @date 2020-02-10
 *  @copyright 2020 - NM Technologies
 */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "ultrasonicController.hpp"

/*--------------------------------------------------/
/                   Macros                          /
/--------------------------------------------------*/

/*--------------------------------------------------/
/                   Globals                         /
/--------------------------------------------------*/
//! Enumerated type based on the hardware names of the ultrasonic sensors
//! \todo Get this string from the JSON file itself, reducing number of dependencies
std::vector<std::string> US_sensorNames = {"HCxSR04_Front", 
										  "HCxSR04_Back", 
										  "HCxSR04_Right", 
										  "HCxSR04_Left"};

/*--------------------------------------------------/
/                   Structures                      /
/--------------------------------------------------*/

/*--------------------------------------------------/
/                   Abbreviations                   /
/--------------------------------------------------*/
/*
 * US = Ultrasonic_Sensor
 * SC = Sensor Controller (wrapper object)
 */
/*--------------------------------------------------/
/                   Prototypes (Protos in header)   /
/--------------------------------------------------*/
/*! 
	\page page1 testPage
	\tableofcontents
	Leading text.
	\section sec1 Test Section
	This is a test section
 	\name 		Test Name
	\author		Mickey T Da Silva
	\copyright  2020 NM Technologies
	\date 		2020-02-08
	\brief 		Defines hardware abstraction for ultrasonic sensors
*/
sensorController::sensorController(){
	//In     : N/A
    //Out    : N/A (CTOR)
    //Description  : Package sensors with query logic
    
    for( unsigned int iSensor = 0; iSensor < US_sensorNames.size(); iSensor++ ){				
				
		//Emplace Sensors
		this->US_sensorBank.emplace_back( HCxSR04(US_sensorNames[iSensor], 
												  iSensor, 
												  US_triggerPin ) 
										);  
    }	
	
}

sensorController::~sensorController(){
	//In     : N/A
    //Out    : N/A (DTOR)
    //Description  : Destructor (Unnecessary but included for completeness or debugging)

	//std::cout << "Ah! You've destroyed my ultrasonic sensor!" << std::endl;
}

double sensorController::getDistance( US_pinout selSensor ){
	//In     : selSensor - Sensor choice (datatype determines which sensor
    //Out    : Distance measured by sensor
    //Description  : Get the distance measured by the requested sensor
    
    NMT_log_write(DEBUG, (char *)"> Sensor Request=%d", selSensor);

    
    double measuredDistance = this->US_sensorBank[selSensor].distance();
    
    
	/* Exit Function */    
    NMT_log_write(DEBUG, (char *)"> Measured Distance=%.6f", measuredDistance);
	return measuredDistance;	
}
