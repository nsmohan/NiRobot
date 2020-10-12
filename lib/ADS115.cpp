/** 
 *  @file      ADS115.cpp
 *  @brief     Driver for ADS115 A/D Converter 
 *  @details   Library to allow access/control to ADS115 IC
 *  @author    Nitin Mohan 
 *  @date      October 10, 2020 
 *  @copyright 2020
 */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <iostream>
#include <map>
#include <wiringPi.h>
#include <wiringPiI2C.h>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "ADS115.hpp"
#include "NMT_log.h"
#include "PCA9685.h"

/*--------------------------------------------------/
/                    Macros                         /
/--------------------------------------------------*/
/**@def ADS115_I2C_ADDRESS
 * ADS115 I2C Address */
#define ADS115_I2C_ADDRESS 0x48

/**@def CONFIG_REG_VALUE
 * Default value for Config Reg*/
#define CONFIG_REG_VALUE 0x0183

/**@def RESET_VALUE
 * Value to trigger Reset */
#define RESET_VALUE 0x06

/**@def MAX_OUTPUT
 * Max Conversion Register Value */
#define MAX_OUTPUT 0xFFFF

/* @def GAIN_VOLTAGE
 * FS Gain Voltage */
#define GAIN_VOLTAGE 6.144
/*--------------------------------------------------/
/                Structs/Classes/Enums              /
/--------------------------------------------------*/

/* @enum ads115_registers
 * ADS115 Registers */
typedef enum 
{
    CONVERSION_REG,
    CONFIG_REG,
    LO_THRESH_REG,
    HI_THRESH_REG
}ads115_registers;

/** @map devices
 *  device to pin mapping*/
std::map<std::string, unsigned int> devices;
/*--------------------------------------------------/
/                   Global Varibles                 /
/--------------------------------------------------*/
/** @var MAX_DEVICES
 *  Maximum Number of Devices */
static int MAX_DEVICES = 4;

/** @var BYTE_OFFSET
 *  Base value for first byte of config register*/
static const int BYTE_OFFSET = 12;

/** @var OS_BIT_MASK
 *  Value to Bit Mask the OS Bit */
static const int OS_BIT_MASK = 0x8000;

/*--------------------------------------------------------------------/
/                             Start of Program                        /
/--------------------------------------------------------------------*/

using namespace std;
ADS115::ADS115(RSXA_hw hw_config) 
{
    /*!
     *  @brief     Constructor
     *  @param[in] hw_config
     *  @param[out]
     *  @return    N/A
     */

    NMT_log_write(DEBUG, (char *)"> ");

    /* 1 .Check Pre-Conditions */
    if (hw_config.array_len_hw_int > MAX_DEVICES)
    {
        NMT_log_write(ERROR, (char *)"Too many devices provided!. Max=%d, Actual=%d",
                     hw_config.array_len_hw_int,
                     MAX_DEVICES);
        throw std::runtime_error("Error! Execeded Max Device Limit");
    }

    /* 2. Initialize Variables */
    this->sim_mode = hw_config.hw_sim_mode;
    this->hw_name  = hw_config.hw_name;

    /* 3. Populate Device List */
    for (int i = 0; i < hw_config.array_len_hw_int; i++)
    {
        devices[hw_config.hw_interface[i].pin_name] = hw_config.hw_interface[i].pin_no;
    }

    /* 4. Setup I2C */
    if (!this->sim_mode) {this->fd = wiringPiI2CSetup(ADS115_I2C_ADDRESS);}
    else {this->fd = 1;}

    if (this->fd < 1){throw std::runtime_error("Error! Unable initialize I2C communication");}

    /* Exit the Function */
    NMT_log_write(DEBUG, (char *)"< ");
}

/*--------------------------------------------------/
/                   Public Methods                  /
/--------------------------------------------------*/
double ADS115::ADS115_get_voltage(std::string hw_name)
{
    /*!
     *  @brief     Read voltage from conversion register and return
     *  @param[in] hw_name
     *  @return    Voltage read
     */

    NMT_log_write(DEBUG, (char *)"> ");

    /* Initialize Varibles */
    double voltage = 0;

    if (this->sim_mode)
    {
        voltage = 6.00;
    }
    else
    {
        /* Begin Conversion for Channel */
        ADS115_begin_conversion_for_input(hw_name);

        /* Wait for Conversion to be Complete */
        ADS115_wait_for_converstion_to_complete();

        /* Get Voltage */
        voltage = ADS115_retrive_voltage_from_register();
    }

    /* Exit the Function */
    NMT_log_write(DEBUG, (char *)"< voltage=%.4f", voltage);
    return voltage;
}

void ADS115::ADS115_reset(void) 
{
    /*!
     *  @brief     Reset the ADS115 Chip
     *  @return    N/A
     */
    NMT_log_write(DEBUG, (char *)"> ");

    /* Reset the IC */
    wiringPiI2CWriteReg8(this->fd, CONVERSION_REG, RESET_VALUE);

    /* 10ms delay to allow things to settle */
    delay(10);

    /* Set the Config Register */ 
    wiringPiI2CWriteReg16(this->fd, CONFIG_REG, CONFIG_REG_VALUE);

    /* Exit the Function */
    NMT_log_write(DEBUG, (char *)"< ");
}

/*--------------------------------------------------/
/                   Private Metods                  /
/--------------------------------------------------*/
void ADS115::ADS115_begin_conversion_for_input(std::string hw_name)
{
    /*!
     *  @brief     Update Config Register for appropriate Channel
     *  @param[in] hw_name
     *  @return    The returned voltage
     */

    NMT_log_write(DEBUG, (char *)"> ");
    
    /* Read Conversion Reg */
    uint16_t reg_val = wiringPiI2CReadReg16(this->fd, CONFIG_REG);
    NMT_log_write(DEBUG, (char *)"config_register_value=%X", reg_val);

    /* Calculate the New Byte Value */
    uint16_t new_reg_val = ((devices[hw_name] + BYTE_OFFSET) << 8) | reg_val;
    NMT_log_write(DEBUG, (char *)"Update Config Register Value=%X", new_reg_val);

    /* Write new value to the config register */
    wiringPiI2CWriteReg16(this->fd, CONFIG_REG, new_reg_val);

    /* Exit the function */
    NMT_log_write(DEBUG, (char *)"< ");
}

double ADS115::ADS115_retrive_voltage_from_register()
{
    /*!
     *  @brief     Read the Conversion register 
     *  @return    Voltage read from regiters
     */

    NMT_log_write(DEBUG, (char *)"> ");

    /* Read the register */
    int16_t conversion_reg_val = wiringPiI2CReadReg16(this->fd, CONVERSION_REG);

    /* Convert Register value to voltage */
    double voltage = GAIN_VOLTAGE * (conversion_reg_val/MAX_OUTPUT);

    /* Exit the Function */
    NMT_log_write(DEBUG, (char *)"< %4.f", voltage);
    return voltage;
}

void ADS115::ADS115_wait_for_converstion_to_complete() 
{
    /*!
     *  @brief     Check the osBit for conversion to be complete
     *  @return    N/A
     */

    NMT_log_write(DEBUG, (char *)"> ");

    /* Initialize Variables */
    int osBit = 0;

    /* Wait for Conversion to be complete */
    while (!osBit)
    {
        /* Get OS Bit State */
        osBit = (wiringPiI2CReadReg16(this->fd, CONFIG_REG) & 0x8000) >> 15;
        delay(100);
    }

    /* Exit the Function */
    NMT_log_write(DEBUG, (char *)"< ");
}
