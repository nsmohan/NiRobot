/*MTDR.c:     Motor Driver. Responsible for directly generating the signals to control the motors
              and provide an API for the application layer. 

__author__      = "Nitin Mohan
__copyright__   = "Copy Right 2019. NM Technologies" */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <stdio.h>
#include <wiringPi.h>
#include <stdlib.h>


/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/

/*--------------------------------------------------/
/                   Constants                       /
/--------------------------------------------------*/
#define PWM_pin 1

int main(int argc, char *argv[]){

    int f;
    double duty_cycle;

    if (wiringPiSetup() == -1){
        printf("Error: Setting up Wiring Pi Failed!\n");
        return -1;
    }

    pinMode(PWM_pin, PWM_OUTPUT);
    pwmWrite(PWM_pin, 512);
    return 0;
}
