/* get_distance.cpp: External Interface to query ultrasonic sensors to get distance data. 

__author__      = "Mickey T Da Silva"
__copyright__   = "Copyright 2020. NM Technologies" */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <iostream>
#include <string>
#include <vector>

#include "getopt.h"
#include "unistd.h"

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "ultrasonicController.hpp"
#include "NMT_stdlib.h"
#include "NMT_log.h"

/*--------------------------------------------------/
/                   Macros                          /
/--------------------------------------------------*/
#define LOG_DIR "/var/log/NiRobot"


using std::cout;
using std::endl;
using std::string;
using std::vector;
/*--------------------------------------------------/
/                   Global Vars                     /
/--------------------------------------------------*/

/*--------------------------------------------------/
/                   Structures                      /
/--------------------------------------------------*/
static struct option long_options[] =
{
    {"sensor",               required_argument, NULL, 's'},
    {"help",                 no_argument,       NULL,  0},
    {NULL, 0, NULL, 0}
};

/*--------------------------------------------------/
/                   Prototypes                      /
/--------------------------------------------------*/
static NMT_result sensorControl_querySensors(std::string selSensor, double* returnedDistance, bool verbosity);
static US_pinout US_queryStrToEnum( std::string command, bool verbosity );
static void US_ctrl_print_usage(int es);


int main( int argc, char** argv ){

    //Input     : Direction String
    //Output    : Direction ENUM
    //Function  : Convert direction string to enum


    /*Initialize Variables */
    NMT_result 	result = OK;
    bool       	verbosity = false;
    int        	longindex = 0;
    int        	opt;
    double returnedDistance;
    string queriedSensor; 


	while ((opt = getopt_long(argc, argv, "s:hv", long_options, &longindex)) != -1){
        switch (opt){
            case 's':
                queriedSensor = optarg;
                break;
            case 'v':
                cout << "Run in verbose mode ................." << endl;
                verbosity = true;
                break;
            case 'h':
                cout << "Help Menu" << endl;
                US_ctrl_print_usage(0);
                break;
            case '?':
                cout << "Unknown Argument Provided" << endl;
                US_ctrl_print_usage(0);
                break;
        }
    }
    
    /* RUN PROGRAM */
    NMT_log_init((char *)LOG_DIR, verbosity);
    result = sensorControl_querySensors(queriedSensor, &returnedDistance, verbosity);
    NMT_log_write(DEBUG, (char *)"< distance=%f", returnedDistance);

	
	return result;
}

static void US_ctrl_print_usage(int es){
    //Input     : exit code
    //Output    : N/A
    //Function  : Print usage text to console and exit gracefully

    std::cout << "Sensor/d (Front/Back/Left/Right) || -v verbosity || -h/help menu" << std::endl;
    exit( es );
}

static NMT_result sensorControl_querySensors( string s_selSensor, double* returnedDistance, bool verbosity ){
	//Input     : String with sensor choice
    //Output    : N/A
    //Function  : Command sensor to take distance data
    
	/* Initialize Variables */
	NMT_result result = OK;
	double sensorDistance;
	US_pinout queriedSensor;
	
	/* Create sensor object */
	sensorController sensorBank; 
	
	NMT_log_write(DEBUG, (char *)"> US_sensor=%s", s_selSensor.c_str());
	
	/* Get sensor to query */
	queriedSensor = US_queryStrToEnum( s_selSensor, verbosity );
	
	/* Get sensor data */
	sensorDistance = sensorBank.getDistance( queriedSensor );
	(*returnedDistance) = sensorDistance;
	
	/* Exit */
	NMT_log_write(DEBUG, (char *)"< result=%s", result_e2s[result]);
	return result;
	
}

static US_pinout US_queryStrToEnum( string command, bool verbosity ){
    //Input     : US Query String
    //Output    : US Query ENUM
    //Function  : Convert US Query string to enum
    
    
    /*Initialize Variables */
    US_pinout queriedSensor;
    
    NMT_log_write( DEBUG, (char*)"> Sensor=%s", command.c_str() );
    
    /* Convert String to Enum */
    vector<string> options = {"Front", "Back", "Left", "Right"};
    
    
    for( unsigned int selOpt = 0; selOpt < options.size(); selOpt++ ){
		if( command.compare( options[selOpt] ) == 0 ){
						
			queriedSensor = (US_pinout) selOpt;
			break;
			
		}
    }
        
    NMT_log_write( DEBUG, (char*) "< sensorEnum=%d", queriedSensor );
    
    return static_cast<US_pinout>( queriedSensor );

}
