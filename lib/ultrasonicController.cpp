/*HCxSR04_Driver.cpp: Driver for the Ultransonic Sensors (HC-SR04)

__author__      = "Mickey T Da Silva"
__copyright__   = "Copyright 2020. NM Technologies" */

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

std::vector<std::string> US_sensorNames = {"HCxSR04_Front", 
										  "HCxSR04_Back", 
										  "HCxSR04_Right", 
										  "HCxSR04_Left"};
bool debugPrint = true;

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

sensorController::sensorController(){
	//In     : N/A
    //Out    : N/A (CTOR)
    //Description  : Package sensors with query logic
    
    for( unsigned int iSensor = 0; iSensor < US_sensorNames.size(); iSensor++ ){
		//debug printing
		if( debugPrint ){
			printf("Adding %s [%d, %d]\n", US_sensorNames[iSensor].c_str(), 
										   iSensor,
										   US_triggerPin);
		}
		
		//Create sensor object
		HCxSR04 curSensor(US_sensorNames[iSensor], 
						  iSensor, 
						  US_triggerPin );
		
		//debug printing				  
		if( debugPrint ){
			printf("Successfully created %s\n", US_sensorNames[iSensor].c_str()); 
		}
		
		//Push it
		this->US_sensorBank.push_back( curSensor ); 
    }	
	
}

sensorController::~sensorController(){
	std::cout << "Ah! You've destroyed my ultrasonic sensor!" << std::endl;
}

double sensorController::getDistance( US_pinout selSensor ){
	return this->US_sensorBank[selSensor].distance();	
}
