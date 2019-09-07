/*register_dump.c: This program will read all the registers when provided with 
                   I2C Address

__author__       = "Nitin Mohan
__copyright__    = "Copy Right 2019. NM Technologies" */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_stdlib.h"

#define MAX_ADDR    0xFF
static void register_dump_getreg(int i2c_address);
static void print_usage(int es);

int main(int argc, char *argv[])
{
    //Initialize Variables
    int opt;
    int address = 0;

    //Parse input arguments and take appropriate action
    if (argc == 3)
    {
        while ((opt = getopt(argc, argv, "ha:")) != -1)
        {
            switch (opt)
            {
                case 'a':
                    sscanf(optarg, "%x", &address);
                    printf("Reading registers on Address: %x\n", address);
                    register_dump_getreg(address);
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
    return 0;
}

static void print_usage(int es)
{
    printf("Address of slave device -a <address> || help -h\n");
    exit(es);
}

static void register_dump_getreg(int i2c_address)
{
    //Initialize Varibles
    int fd            = 0;
    int reg_value     = 0;

    //Initialize I2C Communication
    wiringPiSetup();
    fd = wiringPiI2CSetup(i2c_address);

    //Check if found the slave address
    if (fd < 0)
    {
        printf("Slave Not found!\n");
    }
    else
    {
        for (int address = 0x00; address <= MAX_ADDR; address++)
        {
            reg_value  = wiringPiI2CReadReg8(fd, address);
            if (address != 0x00 && (address % 16) == 0)
                printf("\n");

            if (reg_value != -1)
                printf("%02x ", reg_value);
            else
                printf("## ");
        }
        printf("\n");
    }
}
