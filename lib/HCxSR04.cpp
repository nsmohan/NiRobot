/*HCxSR04_Driver.cpp: Driver for the Ultransonic Sensors (HC-SR04)

__author__      = "Nitin Mohan
__copyright__   = "Copy Right 2019. NM Technologies" */

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

/*--------------------------------------------------/
/                   Structures                      /
/--------------------------------------------------*/

//------------------Prototypes-------------------//


using namespace std;
HCxSR04::HCxSR04(string hw_name, int trigger, int echo, bool sim_mode)
{
    /* Constructor */
    this->trigger = trigger;
    this->echo = echo;
    this->hw_name = hw_name;
    this->sim_mode = sim_mode;
    this->init_hw();
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
