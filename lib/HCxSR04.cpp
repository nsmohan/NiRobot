/** 
 *  @file       HCxSR04.cpp
 *  @brief     Implementation file for HCxSR04.cpp
 *  @details   External interfaces to query ultrasonic sensors
 *  @author    Nitin Mohan
 *  @date      Feb 7, 2019
 *  @copyright 2020 - NM Technologies
 */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <iostream>
#include <chrono>
#include <wiringPi.h>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "HCxSR04.hpp"
#include "NMT_log.h"
#include "RSXA.h"

//---------------------Macros----------------------//
//! @def Hardware string archetype
#define hwStrArch "HCxSR04_"

#define RS_SETTINGS_PATH "/etc/NiBot/RSXA.json"
/*--------------------------------------------------/
/                   Structures                      /
/--------------------------------------------------*/

//------------------Prototypes-------------------//


using namespace std;
HCxSR04::HCxSR04( string echoDir )
{
    //! @var hw_config - Info from RSXA.json
    RSXA* hw_config = new RSXA;
    RSXA_init( hw_config );
    NMT_log_write(DEBUG, (char *)"> JSON hardware length=%d ", hw_config->array_len_hw);

    
    string fullHWName = hwStrArch + echoDir;
    int hwInd = 0; 
    
    //Get hardware specific content from JSON file
    for( int iHardware = 0; iHardware < hw_config->array_len_hw; iHardware++ ){
        string currentHW( hw_config->hw[iHardware].hw_name );
        NMT_log_write(DEBUG, (char *)"> searchingFor %s in %s ", hwStrArch, currentHW.c_str());
        
        size_t foundHW = currentHW.find( hwStrArch );
        NMT_log_write(DEBUG, (char *)"> strPos=%lu ", foundHW);
        if( foundHW != string::npos ){
            NMT_log_write(DEBUG, (char *)"> found hardware type ");
            
            size_t foundSpecHW = currentHW.find( echoDir );
            NMT_log_write(DEBUG, (char *)"> specificStrPos=%lu ", foundSpecHW);
            if( foundSpecHW != string::npos ){
                NMT_log_write(DEBUG, (char *)"> found specific hardware ");
                hwInd = iHardware;
                break;
            }
        }
    }
    
    NMT_log_write(DEBUG, (char *)"> foundHWIndex=%d ", hwInd);
    NMT_log_write(DEBUG, (char *)"> hwName=%s ", hw_config->hw[hwInd].hw_name );
    
    //Populate object
    this->trigger   = hw_config->hw[hwInd].hw_interface[1].pin_no;
    this->echo      = hw_config->hw[hwInd].hw_interface[0].pin_no;
    this->hw_name   = hw_config->hw[hwInd].hw_name;
    this->sim_mode  = hw_config->hw[hwInd].hw_sim_mode;
    
    this->init_hw();
    
    //Cleanup
    NMT_log_write(DEBUG, (char *)"> HCxSR04 successfully constructed ");

    delete hw_config; 
} 


void HCxSR04::init_hw()
{
    //Input     : N/A
    //Output    : N/A
    //Function  : Check simulation mode and init the hardware

    /* Initialize Hardware if not in sim mode */
    if (!(this->sim_mode))
    {
        pinMode(this->trigger, OUTPUT);
        pinMode(this->echo, INPUT);
        digitalWrite(this->trigger, LOW);
        delay(500);
    }
}

double HCxSR04::distance()
{
    //Input     : N/A
    //Output    : distance to object
    //Function  : Ping HCxSR04 sonar and return the distance to 
    //            object

    NMT_log_write(DEBUG, (char *)"> Sonar=%s", this->hw_name.c_str());

    /* Wait for hardware to settle */
    delay(10);


    if (this->sim_mode)
    {
        this->endTimeUsec = 30.00;
        this->startTimeUsec = 20.00;
    }
    else
    {
        /*Send the tigger pulse */
        digitalWrite(this->trigger, HIGH);
        delayMicroseconds(10);
        digitalWrite(this->trigger, LOW);

        /*Wait for echo pin to turn high and 
         * capture the startTime */
        while (digitalRead(this->echo) == LOW)
            this->startTimeUsec = micros();

        /*Wait for echo pin to turn low and 
         * capture the endTime */
        while (digitalRead(this->echo) == HIGH)
            this->endTimeUsec = micros();
    }

    /* Calculate the duration */
    this->travelTimeUsec = this->endTimeUsec - this->startTimeUsec;

    /* distance = Velocity * (Time/2)
     * Speed of sound  = 342 m/s = Velocity */
    /* Calculate the distance */
    distanceMeters = 100 * 171.5 * (this->travelTimeUsec/1000000.00);

    /*Exit the function */
    NMT_log_write(DEBUG, (char *)"> distance=%.6f", distanceMeters);
    return distanceMeters;
}
