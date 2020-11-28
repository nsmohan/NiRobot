/** 
 *  @file      SensorDataAbstraction.hpp
 *  @brief     Sensor Data Abstraction Header File 
 *  @details   External Interfaces for Sensor Data Abstraction
 *  @author    Nitin Mohan 
 *  @date      November 26, 2020 
 *  @copyright 2020
 */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "RSXA.h"
#include "HCxSR04.hpp"
#include "ADS115.hpp"

/*--------------------------------------------------/
/                    Macros                         /
/--------------------------------------------------*/
#define HCxSR04_str "HCxSR04"
#define echoPin_str "echoPin"
#define trigPin_str "trigPin"
#define ADS115_str  "ADS115"

/*--------------------------------------------------/
/                   Globals                         /
/--------------------------------------------------*/
static const std::string BATTERY_VOLTAGE  = "BATTERY_VOLTAGE";
static const std::string PI_INPUT_VOLTAGE = "PI_INPUT_VOLTAGE";
/*--------------------------------------------------/
/                Structs/Classes/Enums              /
/--------------------------------------------------*/
typedef enum DataMode {VOLTAGE, ULTRASONIC, ALL} DataMode;
const std::string DataMode_e2s[] = {"VOLTAGE", "ULTRASONIC", "ALL"};

/*--------------------------------------------------------------------/
/                             Start of Program                        /
/--------------------------------------------------------------------*/
class SensorDataAbstraction
{
    public:
        /* Constructor for SensorDataAbstraction */
        SensorDataAbstraction(RSXA hw_settings, DataMode mode);
        ~SensorDataAbstraction();

   private:
        DataMode mode;
        void init_ultrasonic_sensors(RSXA_hw hw_element);
        void init_voltage_detector(RSXA_hw hw_element);

   protected:
       std::map<std::string, HCxSR04*> ultrasonicSensors;
       ADS115 *voltageSensor;
};

