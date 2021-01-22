/** 
 *  @file      HCxSR04.cpp
 *  @brief     Library to interface with Ultrasonic Sensors
 *  @details   This library provides interfcaes to interact with 
 *             HCxSR04 Ultrasonic Sensors
 *  @author    Nitin Mohan
 *  @date      August 13, 2020
 *  @copyright 2020 - NM Technologies
 *
 *  @History DD/MM/YY Author Description
 *           13/08/20 NITM   Add Doxygen and getter functions for private Variables
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

/*--------------------------------------------------/
/                  MACROS                           /
/--------------------------------------------------*/
#define MAX_WAIT_TIME 23000

/*--------------------------------------------------/
/                   Start of Program                /
/--------------------------------------------------*/
using namespace std;
void HCxSR04::init_hw()
{
    /*!
     *  @brief     Initialize the hardware based on SIM mode
     *  @param[in] N/A
     *  @return    void
     */

    /* Initialize Hardware if not in sim mode */
    if (!(sim_mode))
    {
        wiringPiSetup();
        pinMode(trigger, OUTPUT);
        pinMode(echo, INPUT);
        digitalWrite(trigger, LOW);
        delay(500);
    }
}

double HCxSR04::distance()
{
    /*!
     *  @brief     Ping HCxSR04 sonar and Calculate and return the distance
     *  @param[in] N/A
     *  @return    distanceMeters (meters)
     */

    NMT_log_write(DEBUG, (char *)"> Sonar=%s", hw_name.c_str());

    /* Initialize Variables */
    volatile unsigned long startTimeUsec = 0;
    volatile unsigned long endTimeUsec   = 0;
    unsigned long travelTimeUsec         = 0;
    unsigned long startTimeInitial       = 0;

    /* Wait for hardware to settle */
    delay(10);


    if (sim_mode)
    {
        endTimeUsec = 30.00;
        startTimeUsec = 20.00;
    }
    else
    {
        /*Send the tigger pulse */
        digitalWrite(trigger, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigger, LOW);

        /*Wait for echo pin to turn high and 
         * capture the startTime */

        startTimeInitial = micros();
        startTimeUsec = micros();
        while ((digitalRead(echo) == LOW) && ((startTimeUsec - startTimeInitial) < MAX_WAIT_TIME))
            startTimeUsec = micros();

        /*Wait for echo pin to turn low and 
         * capture the endTime */
        while (digitalRead(echo) == HIGH)
            endTimeUsec = micros();
    }

    /* Calculate the duration */
    NMT_log_write(DEBUG, (char *)"start=%luus end=%luus", startTimeUsec, endTimeUsec);
    travelTimeUsec = endTimeUsec - startTimeUsec;

    /* distance = Velocity * (Time/2)
     * Speed of sound  = 342 m/s = Velocity */
    /* Calculate the distance */
    distancecm = 100 * 171.5 * (travelTimeUsec/1000000.00);

    /*Exit the function */
    NMT_log_write(DEBUG, (char *)"< Sonar=%s distance=%.6fcm", hw_name.c_str(), distancecm);
    return distancecm;
}
