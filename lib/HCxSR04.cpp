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
    if (!(this->sim_mode))
    {
        wiringPiSetup();
        pinMode(this->trigger, OUTPUT);
        pinMode(this->echo, INPUT);
        digitalWrite(this->trigger, LOW);
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
