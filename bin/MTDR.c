/*MTDR.c:       Motor Driver

__author__      = "Nitin Mohan
__copyright__   = "Copy Right 2019. NM Technologies" */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <getopt.h>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_log.h"
#include "MTDR.h"

//---------------------Macros----------------------//
#define LOG_DIR       "/var/log/NiRobot"
#define MAX_ANGLE     270.00
#define MIN_ANGLE     0.00
#define LD27MG_FREQ   50.00
#define LD27MG_OFFSET 0.5
#define LD27MG_SLOPE  135

/*--------------------------------------------------/
/                   Structures                      /
/--------------------------------------------------*/
static struct option long_options[] =
{
    {"motor",                required_argument, NULL, 'm'},
    {"angle",                required_argument, NULL, 'a'},
    {"current_position",     no_argument,       NULL, 'c'},
    {"help",                 no_argument,       NULL,  0},
    {NULL, 0, NULL, 0}
};

//------------------Prototypes-------------------//
static double mtdr_get_duty_cycle(double angle);
NMT_result mtdr_m2c(char *motor, PCA9685_PWM_CHANNEL *channel);
static double mtdr_get_angle(double duty_cycle);
static void print_usage(int es);

/*-------Start of Program--------------*/
int main(int argc, char *argv[])
{
    /* Initialize General Variables */
    int opt;
    NMT_result result         = OK;
    int longindex             = 0;
    bool verbosity            = false;
    bool get_current_position = false;
    bool angle_provided       = false;
    double angle              = 0;
    char   motor[50];

    //Parse input arguments and take appropriate action
    if (argc >= 2)
    {
        while ((opt = getopt_long(argc, argv, "m:a:chv", long_options, &longindex)) != -1)
        {
            switch (opt)
            {
                case 'a':
                    angle = atoi(optarg);
                    angle_provided = true;
                    break;
                case 'm':
                    strcpy(motor, optarg);
                    break;
                case 'v':
                    printf("Program will run in verbose mode .........\n");
                    verbosity = true;
                    break;
                case 'c':
                    get_current_position = true;
                    break;
                case 'h':
                    printf("Help Menu\n");
                    print_usage(0);
                    break;
                case '?':
                    printf("Unknown Argument Provided\n");
                    print_usage(1);
                    break;
            }
        }

        /* Initialize the logger */
        if(result == OK)
        {
            result = NMT_log_init(LOG_DIR, verbosity);
        }

        /* Move the actual Motor */
        if(result == OK && angle_provided)
        {
            result = mtdr_move_motor(motor, angle);
        }

        /* Get Motors current postion */
        if(result == OK && get_current_position)
        {
            double current_angle;
            result = mtdr_get_current_position(motor, &current_angle);

            if (result == OK)
            {
                printf("%.2f\n", current_angle);
            }
        }
    }
    else if (argc > 3)
    {
        printf("Too many arguments provided\n");
        print_usage(1);
    }    
    else
    {
        printf("Error: No Arguments provided\n");
        print_usage(1);
    }
    return result;
}

static void print_usage(int es)
{
    printf("--angle/-a Angle to move to ||/&& --motor/-m motor to move \n \
             || --get_current_position/-c get current angle ||/&& -v verbosity || -h\n");
    exit(es);
}

NMT_result mtdr_m2c(char *motor, PCA9685_PWM_CHANNEL *channel)
{
    //Input     : String with motor name
    //Output    : Channel the motor name corresponds to
    //Function  : Convert motor string to appropriate channel

    NMT_log_write(DEBUG, "> motor=%s", motor);

    /* Initialize Variables */
    NMT_result result = OK;
    int motor_size = sizeof(MTDR_motors)/sizeof(MTDR_motors[0]);

    /* Search for the name in struct */
    for (int i = 0; i < motor_size; i++)
    {
        if (!strcmp(motor, MTDR_motors[i].motor))
        {
            *channel = MTDR_motors[i].channel;
            result = OK;
            NMT_log_write(DEBUG, "< channel=%s result=%s",
                          PCA9685_PWM_CHANNEL_e2s[*channel], result_e2s[result]);
            return result;
        }
    }

    /*Exit the function */
    result = NOK;
    NMT_log_write(DEBUG, "< result=%s",result_e2s[result]);
    return result;
}

NMT_result mtdr_get_current_position(char *motor, double *angle)
{
    //Input     : Name of the motor
    //Output    : Angle of the motor
    //Function  : Get the duty_cycle of PWM channel based 
    //            on motor name and calcualte angle
    
    NMT_log_write(DEBUG, "> motor=%s", motor);

    /* Initialize varibles */
    NMT_result result = OK;
    PCA9685_PWM_CHANNEL channel;
    PCA9685_settings    settings = {0};

    /* Get corresponding channel to motor string */
    result = mtdr_m2c(motor, &channel);

    /* Initialize the PCA9685 Driver */
    if (result == OK)
    {
        result = PCA9685_init(&settings);
    }
    else
    {
        NMT_log_write(ERROR, "Unable to find motor=%s in list of known motors", motor);
    }

    /* Get duty cycle */
    if (result == OK)
    {
        result = PCA9685_getPWM(&settings, channel);
    }
    else
    {
        NMT_log_write(ERROR, "PCA9685 Hardware Failure");
    }

    if (result == OK)
    {
        *angle = mtdr_get_angle(settings.duty_cycle);
    }

    NMT_log_write(DEBUG, "< angle=%.2f result=%s", *angle, result_e2s[result]);
    return result;
}
NMT_result mtdr_move_motor(char *motor, double angle)
{
    //Input     : Name of the motor and angle
    //Output    : N/A
    //Function  : Call appropriate function to move the motor to desired position 

    NMT_log_write(DEBUG, "> motor=%s angle=%f", motor, angle);

    /* Initialize varibles */
    NMT_result result = OK;
    PCA9685_PWM_CHANNEL channel;
    PCA9685_settings    settings = {0};
    settings.freq                = LD27MG_FREQ;
    settings.delay_time          = 0;

    /* Get corresponding channel to motor string */
    result = mtdr_m2c(motor, &channel);

    /* Initialize the PCA9685 Driver */
    if (result == OK)
    {
        result = PCA9685_init(&settings);
    }
    else
    {
        NMT_log_write(ERROR, "Unable to find motor=%s in list of known motors", motor);
    }

    /* Set PWM for the corresponding channel */
    if (result == OK)
    {
        settings.duty_cycle = mtdr_get_duty_cycle(angle);
        result = PCA9685_setPWM(&settings, channel);
    }
    else
    {
        NMT_log_write(ERROR, "PCA9685 Hardware Failure");
    }

    NMT_log_write(DEBUG, "< result=%s",result_e2s[result]);
    return result;
}

static double mtdr_get_duty_cycle(double angle)
{
    //Input     : Angle of rotation
    //Output    : On time in milliseconds
    //Function  : Convert the angle of rotation needed to on_time in ms

    NMT_log_write(DEBUG, "> angle: %f",angle);
            
    //Initialize Varibles
    double on_time;
    double off_time;
    double duty_cycle;

    //Ensure the provided angle is within limits
    angle = (angle > MAX_ANGLE ? MAX_ANGLE : (angle < MIN_ANGLE ? MIN_ANGLE : angle));

    //on_time  (ms) = (angle (degrees * SLOPE) + OFFSET
    //off_time (ms) = (1/FREQ) * 1000
    on_time  = (angle/LD27MG_SLOPE) + LD27MG_OFFSET;
    off_time = ((1/LD27MG_FREQ) * 1000);

    duty_cycle = ((on_time/off_time) * 100);

    //Exit the function
    NMT_log_write(DEBUG, "< on_time: %.2fms off_time: %.2fms duty_cycle: %.2f%%",on_time, off_time, duty_cycle);
    return duty_cycle;
}

static double mtdr_get_angle(double duty_cycle)
{
    //Input     : Duty Cycle
    //Output    : Angle
    //Function  : Convert duty_cycle to Angle

    NMT_log_write(DEBUG, "> angle: %f",duty_cycle);
            
    //Initialize Varibles
    double on_time;
    double off_time;
    double angle;

    //off_time (ms) = (1/FREQ) * 1000
    off_time = ((1/LD27MG_FREQ) * 1000);
    on_time = (duty_cycle/100) * off_time;

    angle = (on_time - LD27MG_OFFSET) * LD27MG_SLOPE;

    //Exit the function
    NMT_log_write(DEBUG, "< on_time: %.2fms off_time: %.2fms angle: %.2f%%",on_time, off_time, angle);

    return angle;
}
