/** 
 *  @file       max1704x.cpp
 *  @brief      MAX1704X Library 
 *  @details    This Library provides interfaces to configure and control the MAX1704X IC 
 *  @author     Nitin Mohan
 *  @date       July 26, 2020 
 *  @copyright  NM Technologies 2020
 */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <iostream>
#include <wiringPi.h>
#include <wiringPiI2C.h>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_log.h"

/*--------------------------------------------------/
/                    Macros                         /
/--------------------------------------------------*/
/**@def MAX1704X_I2C_ADDRESS
 * MAX1704X I2C Address */
#define MAX1704X_I2C_ADDRESS 0x36

/**@def REG_VCELL
 * VCELL Register */
#define REG_VCELL 0x02

/**@def REG_SOC
 * SOC Register */
#define REG_SOC 0x04

/**@def REG_MODE
 * Mode Register */
#define REG_MODE 0x06

/**@def REG_VERSION
 * Version Register */
#define REG_VERSION 0x08

/**@def REG_CONFIG
 *  Config Register */
#define REG_CONFIG 0x0C

/**@def REG_COMMAND
 * Command Register */
#define REG_COMMAND 0xFE

/*--------------------------------------------------/
/                    Global Variables               /
/--------------------------------------------------*/
const double multiplier_043 = 1.25;
const double multiplier_044 = 2.50;

/*--------------------------------------------------/
/                Structs/Classes/Enums              /
/--------------------------------------------------*/

typedef enum {MAX17043, MAX17044}MAX1704X_VERSION;
class MAX1704X
{
    public:
        MAX1704X(bool sim_mode);
        ~MAX1704X();

        double get_voltage_measurement();
        double get_SOC_measurement();

    private:
        uint16_t get_ic_version();
        bool sim_mode;
        int fd;
        double multiplier;
        MAX1704X_VERSION max_chip_version;
};

/*--------------------------------------------------------------------/
/                       Library Implementation                        /
/--------------------------------------------------------------------*/

using namespace std;
MAX1704X::MAX1704X(bool sim_mode)
{
    /*!
     *  @brief     Constructor for MAX1704X
     *  @param[in]
     *  @param[out]
     */

    NMT_log_write(DEBUG, (char *)"> ");

    /* Initialize Variables */
    this->sim_mode = sim_mode;

    if ((!this->sim_mode))
    {
        this->fd = wiringPiI2CSetup(MAX1704X_I2C_ADDRESS);

        if (this->fd < 1) {throw runtime_error("I2C Setup Error");}
        int max_ic_version = wiringPiI2CReadReg16(this->fd, REG_VERSION);
        this->max_chip_version = max_ic_version == 17043 ? MAX17043 : MAX17044;
        this->multiplier = max_chip_version == MAX17043 ? multiplier_043 : multiplier_044;
    }
    else
    {
        /* Configure to run in sim_mode */
        this->fd = 1;
    }

    /* Exit the Function */
    NMT_log_write(DEBUG, (char *)"< ");
}

double MAX1704X::get_voltage_measurement()
{
    /*!
     *  @brief     Read th VCELL register and return the voltage that is read.
     *  return read_voltage -> Voltage read from the register
     */

    NMT_log_write(DEBUG, (char *)"> ");

    /* Read the register value */
    uint16_t vcell_reg_value = (uint16_t)wiringPiI2CReadReg16(this->fd, REG_VCELL) >> 4;
    NMT_log_write(DEBUG, (char *)"vcell_reg_value=%hu", vcell_reg_value);

    /* Exit the function */
    NMT_log_write(DEBUG, (char *)"< ");
    return (double)(vcell_reg_value * this->multiplier);
}

double get_SOC_measurement() 
{
    /*!
     *  @brief     Description
     *  @param[in]
     *  @param[out]
     *  @return    0
     */


    NMT_log_write(DEBUG, (char *)"> ");

    /* Initialize Variables */

    /* Exit the Function */
    NMT_log_write(DEBUG, (char *)"< ");
    return 0;
}

