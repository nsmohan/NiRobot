/*PCA9685.c:  Library for interacting with the PCA9685 PWM Controller

__author__      = "Nitin Mohan
__copyright__   = "Copy Right 2019. NM Technologies" */

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
#include "NMT_log.h"
#include "PCA9685.h"
#include "RSXA.h"

/*--------------------------------------------------/
/                   Constants                       /
/--------------------------------------------------*/
#define HW_NAME "PCA9685_PWM_DRIVER"

//I2C address of PCA9685
#define I2C_ADDRESS 0x40

//Address of Mode Registers
#define MODE1       0x00
#define MODE2       0x01
#define PRE_SCALE   0xFE

//Register Values
#define MODE1_INIT  0x21
#define MODE2_INIT  0x04
#define WAKE_UP     0x10

//Address of first O/P register (Remaning are calculated)
#define LED0_ON_L   0x06

//OSC_CLOCK for PCA9685 is 25MHz and max tics is 4096
#define OSC_CLOCK   25000000
#define MAX_TICS    4096

//------------------Prototypes----------------------//
static NMT_result PCA9685_setFreq(PCA9685_settings *settings, bool sim_mode);

NMT_result PCA9685_init(PCA9685_settings *settings)
{
    //Input     : PCA9685 settings structure
    //Output    : fd (Pointer to access IC via I2C)
    //Function  : Intialize the PCA9685 device based on the settings provided

    //Initialize Varibles
    NMT_result result   = OK;
    bool       sim_mode = false;

    NMT_log_write(DEBUG, "> freq: %f duty_cycle: %f delay_time: %f",settings->freq, settings->duty_cycle,
                                                                    settings->delay_time);
    /* Check if we're in simulation mode */
    result = RSXA_get_mode(HW_NAME, &sim_mode);
    NMT_log_write(DEBUG, "hw_name=%s sim_mode=%s", HW_NAME, btoa(sim_mode));

    if ((result == OK) && (!sim_mode))
    {
        //Initialize I2C Communication
        wiringPiSetup();
        settings->fd = wiringPiI2CSetup(I2C_ADDRESS);
    }
    else
    {
        settings->fd = 1;
    }

    //Check if found the slave address
    if (settings->fd < 0)
        return result = NOK;

    //Set prescale freq
    PCA9685_setFreq(settings, sim_mode);

    if ((result == OK) && (!sim_mode))
    {
        //Setup Mode1 & Mode2 Registers
        //Mode-1: Enable Auto-Increment and wake-up the device
        //Mode-2: Outputs configured as totem pole-structure
        wiringPiI2CWriteReg8(settings->fd, MODE1, MODE1_INIT);
        wiringPiI2CWriteReg8(settings->fd, MODE2, MODE2_INIT);

        //Set the init done flag
        (result == OK) ? (settings->init_done = true) : (settings->init_done = false);
    }

    NMT_log_write(DEBUG, "< %s fd: %d",result_e2s[result], settings->fd);
    return result;
}

NMT_result PCA9685_chgFreq(PCA9685_settings *settings)
{
    //Input     : PCA9685 settings structure
    //Output    : N/A
    //Function  : Changed the frequency to the one provided in the input struct

    //Initialize Varibles
    NMT_result result = OK;
    bool sim_mode     = false;
    int orig_reg_value;
    int sleep_reg_value;

    /* Check if we're in simulation mode */
    result = RSXA_get_mode(HW_NAME, &sim_mode);
    NMT_log_write(DEBUG, "hw_name=%s sim_mode=%s", HW_NAME, btoa(sim_mode));

    if ((result == OK) && (!sim_mode))
    {
        //Check if we have a valid slave address
        if (settings->fd < 0)
            return result = NOK;

        //Read current register value and set bit to put chip into sleep mode
        orig_reg_value  = wiringPiI2CReadReg8(settings->fd, MODE1);
        sleep_reg_value = orig_reg_value | WAKE_UP;

        //Write new value to the register
        wiringPiI2CWriteReg8(settings->fd, MODE1, sleep_reg_value);

        //Set prescale freq
        PCA9685_setFreq(settings, sim_mode);

        //Let the Oscillator settle 1ms
        delay(1);

        //Wake-up device
        wiringPiI2CWriteReg8(settings->fd, MODE1, orig_reg_value);
    }

    //Exit Function
    return result;
}

static NMT_result PCA9685_setFreq(PCA9685_settings *settings, bool sim_mode)
{
    //Input     : PCA9685 settings structure
    //Output    : N/A
    //Function  : Set the frequency to the one provided in the input struct

    //Initialize Varibles
    NMT_result result = OK;

    NMT_log_write(DEBUG, "> freq: %f duty_cycle: %f delay_time: %f fd: %d",settings->freq, settings->duty_cycle,
                                                                           settings->delay_time, settings->fd);
    //Check if we have a valid slave address
    if (settings->fd < 0)
        return result = NOK;

    //Cap max freq to 1500 and min to 30
    settings->freq = (settings->freq > 1500 ? 1500 : (settings->freq < 30 ? 30 : settings->freq));

    //Calculate prescale value. 
    // PRE_SCALE = (OSC_CLOCK/(4096 * freq)) - 1
    int pre_scale = (int)(OSC_CLOCK / (MAX_TICS * settings->freq) - 1);
    
    if(!sim_mode)
    {
        //Write prescale value to register
        wiringPiI2CWriteReg8(settings->fd, PRE_SCALE, pre_scale);
    }

    NMT_log_write(DEBUG, "< %s pre_scale: %d",result_e2s[result], pre_scale);

    //Exit Function
    return result;
}

NMT_result PCA9685_setPWM(PCA9685_settings *settings, PCA9685_PWM_CHANNEL channel)
{
    //Input     : PCA9685 settings structure and PWM channel
    //Output    : N/A
    //Function  : Set PWM duty cycle on the desired channel

    //Initialize Varibles
    NMT_result result = OK;
    bool sim_mode     = false; 

    NMT_log_write(DEBUG, "> freq: %f duty_cycle: %f delay_time: %f fd: %d channel: %d",
                  settings->freq, settings->duty_cycle, settings->delay_time, settings->fd, 
                  channel);

    /* Check if we're in simulation mode */
    result = RSXA_get_mode(HW_NAME, &sim_mode);
    if (result == OK)
    {
        NMT_log_write(DEBUG, "hw_name=%s sim_mode=%s", HW_NAME, btoa(sim_mode));

        //Check if we have a valid slave address
        if (settings->fd < 0)
            return result = NOK;

        //Cap max delay to 100 and min to 0
        settings->duty_cycle = (settings->duty_cycle > 100 ? 100 : 
                (settings->duty_cycle < 0 ? 0 : settings->duty_cycle));
        settings->delay_time = (settings->delay_time > 100 ? 100 : 
                (settings->delay_time < 0 ? 0 : settings->delay_time));

        //Calculate number of tics for time on & off
        int tics_to_on       = (((settings->delay_time/100)*MAX_TICS) + 0.5) - 1;
        int tics_on_duration = (((settings->duty_cycle/100)*MAX_TICS) + 0.5);
        int tics_to_off      = tics_to_on + tics_on_duration; 

        //Calculate the register address
        int channel_reg_on  = (channel * 4) + LED0_ON_L;
        int channel_reg_off = channel_reg_on + 2;

        NMT_log_write(DEBUG, "tics_to_on:%d tics_on_duration:%d tics_to_off:%d  \
                              channel_reg_on:%X channel_reg_off:%X", 
                              tics_to_on, tics_on_duration, tics_to_off, 
                              channel_reg_on, channel_reg_off);
        if (!sim_mode)
        {
            //Write to the registers
            wiringPiI2CWriteReg16(settings->fd, channel_reg_on,  tics_to_on);
            wiringPiI2CWriteReg16(settings->fd, channel_reg_off, tics_to_off);
        }
    }

    NMT_log_write(DEBUG, "< %s", result_e2s[result]);

    //Exit function
    return result;
}

NMT_result PCA9685_getPWM(PCA9685_settings *settings, PCA9685_PWM_CHANNEL channel)
{
    //Input     : PCA9685 settings structure and PWM channel
    //Output    : N/A
    //Function  : Get PWM duty cycle on the desired channel

    //Initialize Varibles
    NMT_result result = OK;
    bool sim_mode     = false; 
    int tics_on_duration;

    NMT_log_write(DEBUG, "> channel=%s", PCA9685_PWM_CHANNEL_e2s[channel]);

    /* Check if we're in simulation mode */
    result = RSXA_get_mode(HW_NAME, &sim_mode);

    if(result == OK)
    {
        NMT_log_write(DEBUG, "hw_name=%s sim_mode=%s", HW_NAME, btoa(sim_mode));

        //Check if we have a valid slave address
        if (settings->fd < 0)
            return result = NOK;

        if (!sim_mode)
        {
            //Calculate the register address
            int channel_reg_on  = (channel * 4) + LED0_ON_L;
            int channel_reg_off = channel_reg_on + 2;
            
            //Write to the registers
            int tics_to_on  = wiringPiI2CReadReg16(settings->fd, channel_reg_on);
            int tics_to_off = wiringPiI2CReadReg16(settings->fd, channel_reg_off);
            tics_on_duration = tics_to_off - tics_to_on;

        }
        else
        {
            tics_on_duration = 500;
        }

        /* Calculate the duty cycle */
        settings->duty_cycle = ((tics_on_duration - 0.5)/MAX_TICS) * 100;
    }

    //Exit the function
    NMT_log_write(DEBUG, "< duty_cycle=%f", settings->duty_cycle);
    return result;

}
