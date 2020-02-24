/** 
 *  @file       get_distance.cpp
 *  @brief     Program for acquiring ultrasonic sensor data
 *  @details   A program designed to test the ultrasonic sensors
 *  @author    Mickey T Da Silva
 *  @date      Feb 17, 2020
 *  @copyright 2020 - NM Technologies
 */

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
//    {"sim_mode",             no_argument,       NULL, 'S'}, //defined in JSON
    {NULL, 0, NULL, 0}
};

/*--------------------------------------------------/
/                   Prototypes                      /
/--------------------------------------------------*/
static NMT_result sensorControl_querySensors(std::string selSensor, double* returnedDistance, bool verbosity);
static void US_ctrl_print_usage(int es);

int main( int argc, char** argv ){
    /*!
     *  @brief    Main function -> Takes user input and logs distance from queried sensor
     *  @return   status
     */
    
    NMT_result 	result = OK;
    bool       	verbosity = false;
    int        	longindex = 0;
    int        	opt;
    //bool        sim_mode = false; //obtained from JSON

    double returnedDistance;
    string queriedSensor; 


	while ((opt = getopt_long(argc, argv, "s:hvS", long_options, &longindex)) != -1){
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

	//NMT_log_finish();
	return result;
}

static void US_ctrl_print_usage(int es){
    //Input     : exit code
    //Output    : N/A
    //Function  : Print usage text to console and exit gracefully

    std::cout << "Sensor/d (Front/Back/Left/Right) || -v verbosity || -h help menu || -S sim_mode" << std::endl;
    exit( es );
}

static NMT_result sensorControl_querySensors( string s_selSensor, double* returnedDistance, bool verbosity){
	//Input     : String with sensor choice
    //Output    : N/A
    //Function  : Command sensor to take distance data
    
	/* Initialize Variables */
	NMT_result result = OK;
	//double sensorDistance;
	
	/* Create sensor object */
	sensorController US_sensor( s_selSensor ); 
	
	NMT_log_write(DEBUG, (char *)"> US_sensor=%s", s_selSensor.c_str());
		
	/* Get sensor data */
	(*returnedDistance) = US_sensor.getDistance();
	
	/* Exit */
	NMT_log_write(DEBUG, (char *)"< result=%s", result_e2s[result]);
	return result;
	
}
