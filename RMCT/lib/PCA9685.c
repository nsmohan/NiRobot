/** 
 *  @file      PCA9685.c
 *  @brief     Driver library for interfacting with the PCA9685 PWM Controller
 *  @details   This libraries provides methods to configure and control the
 *             PCA9685 PWM Controller
 *  @author    Nitin Mohan
 *  @date      Feb 7, 2019
 *  @copyright 2020 - NM Technologies
 */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMTX/NMT_log.h"
#include "PCA9685.h"
#include "RSXA/RSXA.h"

/*--------------------------------------------------/
/                   Constants                       /
/--------------------------------------------------*/

 /** @def MODE1 
  * Mode1 register address */
#define MODE1       0x00

/** @def MODE2 
 * Mode2 Register address */
#define MODE2       0x01

/** @def PRE_SCALE 
 * PRE_SCALE register address */
#define PRE_SCALE   0xFE

/** @brief Default Register Values
 *  @def MODE1_INIT 
 *  Mode1 Init value */
#define MODE1_INIT  0x21

/**  @def MODE2_INIT 
 * Mode2 init value */
#define MODE2_INIT  0x04

/**  @def WAKE_UP 
 * Register value to wake up register*/
#define WAKE_UP     0x10

/** @def LED0_ON_L
 * Address of first output register
 * Remaning are calculated */
#define LED0_ON_L   0x06

/** @def OSC_CLOCK 
 * Oscillator Frequency */
#define OSC_CLOCK   25000000

/** @def MAX_TICS
 * Max number of tics per cycle */
#define MAX_TICS    4096

/**@def PCA9685_I2C_ADDRESS
 * PCA9685 I2C Address */
#define PCA9685_I2C_ADDRESS 0x40

/*--------------------------------------------------/
/                   Global Varibles                 /
/--------------------------------------------------*/
/** @var FD
 *  File Descripter for WiringPiI2C */
static int FD = -1;

/** @var SIM_MODE
 *  Simulation Mode for PCA9685 PWM Driver */
static bool SIM_MODE = false;

/** @var CURRENT_FREQ
 *  The current PWM Frequency. (This is only set by setFreq */
static float CURRENT_FREQ = 0.00;

//------------------Prototypes----------------------//
static NMT_result PCA9685_setFreq(float freq);

NMT_result PCA9685_init(PCA9685_settings settings)
{
    /*!
     *  @brief     Initialize the PWM Controller based in settings
     *  @param[in] settings
     *  @return    NMT_result
     */

    /* Initialize Variables */
    NMT_result result   = OK;

    NMT_log_write(DEBUG, "> freq: %f" ,settings.freq);

    /* 1: Set the Simulation Mode for the Driver */
    SIM_MODE = settings.sim_mode;

    /* 2. Initialize I2C Communication */
    if (!SIM_MODE) {FD = wiringPiI2CSetup(PCA9685_I2C_ADDRESS);}
    else {FD = 1;}

    /* 3. Check if I2C Init was Successful */
    if (FD < 0)
        result = NOK;

    /* 4. Set the Required Frequency */
    result = PCA9685_setFreq(settings.freq);

    /* 5. Set the PCA9685 PWM Driver Registers */
    if ((result == OK) && (!SIM_MODE))
    {
        /*Setup Mode1 & Mode2 Registers
          *Mode-1: Enable Auto-Increment and wake-up the device
          *Mode-2: Outputs configured as totem pole-structure */
        wiringPiI2CWriteReg8(FD, MODE1, MODE1_INIT);
        wiringPiI2CWriteReg8(FD, MODE2, MODE2_INIT);
    }

    /* Exit the functin */
    NMT_log_write(DEBUG, "< result=%s fd: %d",enum2str(result), FD);
    return result;
}

NMT_result PCA9685_chgFreq(float freq)
{
    /*!
     *  @brief     Change the PWM Frequency
     *  @param[in] freq
     *  @return    NMT_result
     */

    /* Initialize Variables */
    NMT_result result = OK;
    int orig_reg_value;
    int sleep_reg_value;

    NMT_log_write(DEBUG, "> freq: %f", freq);

    /* Check if we have a valid slave address */
    if (FD < 0)
        return result = NOK;

    if ((result == OK) && (!SIM_MODE))
    {
        /* Read current register value and set bit to put chip into sleep mode */
        orig_reg_value  = wiringPiI2CReadReg8(FD, MODE1);
        sleep_reg_value = orig_reg_value | WAKE_UP;

        /* Write new value to the register */
        wiringPiI2CWriteReg8(FD, MODE1, sleep_reg_value);

        /* Set prescale freq */
        PCA9685_setFreq(freq);

        /* Let the Oscillator settle 1ms */
        delay(1);

        /* Wake-up device */
        wiringPiI2CWriteReg8(FD, MODE1, orig_reg_value);
    }

    /* Exit Function */
    NMT_log_write(DEBUG, "< result=%s", enum2str(result));
    return result;
}

static NMT_result PCA9685_setFreq(float freq)
{
    /*!
     *  @brief     Set the frequency based on input
     *  @param[in] freq
     *  @return    NMT_result
     */

    /* Initialize Variables */
    NMT_result result = OK;

    NMT_log_write(DEBUG, "> freq: %f", freq);

    /* Check if we have a valid slave address */
    if (FD < 0)
        return result = NOK;

    /* Cap max freq to 1500 and min to 30 */
    freq = (freq > 1500 ? 1500 : (freq < 30 ? 30 : freq));
    CURRENT_FREQ = freq;

    /* Calculate prescale value. 
     *PRE_SCALE = (OSC_CLOCK/(4096 * freq)) - 1 */
    int pre_scale = (int)(OSC_CLOCK / (MAX_TICS * freq) - 1);
    
    if(!SIM_MODE)
    {
        //Write prescale value to register
        wiringPiI2CWriteReg8(FD, PRE_SCALE, pre_scale);
    }

    NMT_log_write(DEBUG, "< %s pre_scale: %d",enum2str(result), pre_scale);

    //Exit Function
    return result;
}

NMT_result PCA9685_setPWM(double duty_cycle, double delay_time, 
                          PCA9685_PWM_CHANNEL channel)
{
    /*!
     *  @brief     Set PWM duty_cycle on desired channel
     *  @param[in] duty_cycle
     *  @param[in] delay_time
     *  @param[in] channel
     *  @return    NMT_result
     */

    /*Initialize Variables */
    NMT_result result = OK;

    if (FD < 0)
        return result = NOK;

    NMT_log_write(DEBUG, "> freq: %f duty_cycle: %f delay_time: %f fd: %d channel: %d",
                  CURRENT_FREQ, duty_cycle, delay_time, FD, 
                  channel);

    if (result == OK)
    {
        NMT_log_write(DEBUG, "hw_name=%s SIM_MODE=%s", PCA9685_HW_NAME, btoa(SIM_MODE));

        /* Cap max delay to 100 and min to 0 */
        duty_cycle = (duty_cycle > 100 ? 100 : 
                (duty_cycle < 0 ? 0 : duty_cycle));
        delay_time = (delay_time > 100 ? 100 : 
                (delay_time < 0 ? 0 : delay_time));

        /* Calculate number of tics for time on & off */
        int tics_to_on       = (((delay_time/100)*MAX_TICS) + 0.5) - 1;
        int tics_on_duration = (((duty_cycle/100)*MAX_TICS) + 0.5);
        int tics_to_off      = tics_to_on + tics_on_duration -1; 

        /* Calculate the register address */
        int channel_reg_on  = (channel * 4) + LED0_ON_L;
        int channel_reg_off = channel_reg_on + 2;

        NMT_log_write(DEBUG, "tics_to_on:%d tics_on_duration:%d tics_to_off:%d channel_reg_on:%X channel_reg_off:%X", 
                              tics_to_on, tics_on_duration, tics_to_off, 
                              channel_reg_on, channel_reg_off);
        if (!SIM_MODE)
        {
            /* Write to the registers */
            wiringPiI2CWriteReg16(FD, channel_reg_on,  tics_to_on);
            wiringPiI2CWriteReg16(FD, channel_reg_off, tics_to_off);
        }
    }

    /* Exit function */
    NMT_log_write(DEBUG, "< %s", enum2str(result));
    return result;
}

NMT_result PCA9685_getPWM(double *duty_cycle,
                          PCA9685_PWM_CHANNEL channel)
{
    /*!
     *  @brief      Get PWM on provided channel
     *  @param[in]  channel
     *  @param[out] duty_cycle
     *  @return     NMT_result
     */

    /* Initialize Variables */
    NMT_result result = OK;
    int tics_on_duration;

    if (FD < 0)
        return result = NOK;

    NMT_log_write(DEBUG, "> channel=%s", enum2str(channel));

    if (!SIM_MODE)
    {
        //Calculate the register address
        int channel_reg_on  = (channel * 4) + LED0_ON_L;
        int channel_reg_off = channel_reg_on + 2;
        
        //Write to the registers
        int tics_to_on  = wiringPiI2CReadReg16(FD, channel_reg_on);
        int tics_to_off = wiringPiI2CReadReg16(FD, channel_reg_off);
        tics_on_duration = tics_to_off - tics_to_on;
    }
    else
    {
        tics_on_duration = 500;
    }

    /* Calculate the duty cycle */
    *duty_cycle = ((tics_on_duration - 0.5)/MAX_TICS) * 100;

    //Exit the function
    NMT_log_write(DEBUG, "< duty_cycle=%f result=%s", *duty_cycle, enum2str(result));
    return result;

}

NMT_result PCA9685_get_init_status(bool *initialized)
{
    /*!
     *  @brief     Read Config Registers and determine state
     *  @param[in] settings
     *  @param[out] initialized
     *  @return    NMT_result
     */

    //Initialize Variables
    NMT_result result = OK;
    float freq       = 0.0;
    int mode_1_reg    = 0;
    int mode_2_reg    = 0;
    int pre_scale     = 0;

    if (FD < 0)
        return result = NOK;

    NMT_log_write(DEBUG, "> fd=%d", FD);

    if ((result == OK) && (!SIM_MODE))
    {
        /* Get Register Values */
        mode_1_reg  = wiringPiI2CReadReg8(FD, MODE1);
        mode_2_reg  = wiringPiI2CReadReg8(FD, MODE2);
        pre_scale   = wiringPiI2CReadReg8(FD, PRE_SCALE);

        /* Calcualte the Frequency */
        freq = (OSC_CLOCK/(MAX_TICS * (pre_scale + 1)));

        /* Check if PCA9685 Driver is Initialized and set the status flag */
        NMT_log_write(DEBUG, "freq=%.2f mode1=0x%x mode2=0x%02x", freq, mode_1_reg, mode_2_reg);
        if ((MODE1_INIT == mode_1_reg) && (MODE2_INIT == mode_2_reg) && (CURRENT_FREQ == freq))
        {
            *initialized = true;
        }
    }
    else if ((result == OK) && (SIM_MODE))
    {
        /* We always return True in SIM_MODE */
        *initialized = true;
    }

    NMT_log_write(DEBUG, "< initialized=%s result=%s", btoa(*initialized), enum2str(result));
    return result;
}

float PCA9685_get_curret_freq()
{
    /*!
     *  @brief     Return the current Set Frequency of PWM Driver
     *  @return    freq
     */

    NMT_log_write(DEBUG, "> ");
    NMT_log_write(DEBUG, "< freq=%.2f", CURRENT_FREQ);
    return CURRENT_FREQ;
}
