/*mtdr.c:       Motor Driver

__author__      = "Nitin Mohan
__copyright__   = "Copy Right 2019. NM Technologies" */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_log.h"
#include "PCA9685.h"

//---------------------Macros----------------------//
#define LOG_DIR       "/var/log/NiRobot"
#define MAX_ANGLE     270.00
#define MIN_ANGLE     0.00
#define LD27MG_FREQ   50.00
#define LD27MG_OFFSET 0.5
#define LD27MG_SLOPE  (1/135)

//------------------Prototypes-------------------//
typedef enum
{
    CAM_HOZ_MTR = CHANNEL_15,
    CAM_VER_MTR = CHANNEL_14

} motors;


static struct option long_options[] =
{
    {"motor",     required_argument, NULL, 'm'},
    {"angle",     required_argument, NULL, 'a'},
    {"help",      no_argument,       NULL,  0},
    {NULL, 0, NULL, 0}
};

static double mtdr_get_duty_cycle(double angle);
static void print_usage(int es);
NMT_result mtdr_move_motor(PCA9685_settings *settings, motors motor, double angle);

/*-------Start of Program--------------*/
int main(int argc, char *argv[])
{
    

    //Initialize Variables
    PCA9685_settings settings = {0};
    int longindex             = 0;
    double angle              = 1;      //Default value for the angle is set to 1
    bool verbosity            = false;
    int opt;

    //Initialize PCA9685 Driver
    settings.freq = LD27MG_FREQ;
    settings.delay_time = 0;

    //PCA9685_init(&settings);

    //Parse input arguments and take appropriate action
    if (argc >= 3)
    {
        while ((opt = getopt_long(argc, argv, "m:a:hv", long_options, &longindex)) != -1)
        {
            switch (opt)
            {
                case 'a':
                case 'm':
                    printf("%s\n", optarg);
                    //mtdr_move_motor(settings, optarg)
                    break;
                case 'v':
                    printf("Program will run in verbose mode .........\n");
                    verbosity = true;
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
        NMT_log_init(LOG_DIR, verbosity);
    }
    else if (argc > 5)
    {
        printf("Too many arguments provided\n");
        print_usage(1);
    }    
    else
    {
        printf("Error: No Arguments provided\n");
        print_usage(1);
    }
    return 0;


}

static void print_usage(int es)
{
    printf("--angle/-a Angle to move to || --motor/-m motor to move || -h\n");
    exit(es);

}
NMT_result mtdr_move_motor(PCA9685_settings *settings, motors motor, double angle)
{
    //Input     : Settings Object, motor to move, angle
    //Output    : N/A
    //Function  : Call appropriate function to move the motor to desired position 

    //Init Variables
    NMT_result result = OK;

    settings->duty_cycle = mtdr_get_duty_cycle(angle);
    //result = PCA9685_setPWM(&settings, motor);

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
    on_time  = (angle * LD27MG_SLOPE) + LD27MG_OFFSET;
    off_time = ((1/LD27MG_FREQ) * 1000);

    duty_cycle = ((on_time/off_time) * 100);

    //Exit the function
    NMT_log_write(DEBUG, "< on_time: %.2fms off_time: %.2fms duty_cycle: %.2f ms",on_time, off_time, duty_cycle);
    return duty_cycle;
}
