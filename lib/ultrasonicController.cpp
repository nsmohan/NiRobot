/*!
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
/             Prototypes (Protos in header)         /
/--------------------------------------------------*/

using std::string;
sensorController::sensorController(string echoDirection){
	/*!
     * @class sensorController
     * @brief Abstraction layer for querying ultrasonic sensors
     * @param[in] echoDirection - The direction of the ultrasonic sensor to be queried (ex: "Front", "Back", "Left", "Right")
     */
     NMT_log_write( DEBUG, (char*) "> Acquiring %s sensor ", echoDirection.c_str() );
     
     this->echoDirection    = echoDirection;
     this->chosenSensor     = HCxSR04( echoDirection );
     
     NMT_log_write( DEBUG, (char*) "> %s Sensor Acquired ", echoDirection.c_str() );
     
     //Clean Up
     NMT_log_finish();
}

sensorController::~sensorController(){
	//In     : N/A
    //Out    : N/A (DTOR)
    //Description  : Destructor (Unnecessary but included for completeness or debugging)
    
    NMT_log_write( DEBUG, (char*) "> Destroying %s sensor ", this->echoDirection.c_str() );
    
    //cleanup
    NMT_log_finish();
}

double sensorController::getDistance(){
    //In     : selSensor - Sensor choice (datatype determines which sensor
    //Out    : Distance measured by sensor
    //Description  : Get the distance measured by the requested sensor
    
    NMT_log_write(DEBUG, (char *)"> Sensor Request=%s", this->echoDirection.c_str());


    double measuredDistance = this->chosenSensor.distance();


    /* Exit Function */    
    NMT_log_write(DEBUG, (char *)"> Measured Distance=%.6f", measuredDistance);
    NMT_log_finish();
    return measuredDistance;	
}
