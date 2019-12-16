#ifndef DEF_HCxSR04_Driver
#define DEF_HCxSR04_Driver

/*HCxSR04.hpp: Header File for HCxSR04.cpp

__author__      = "Nitin Mohan
__copyright__   = "Copy Right 2019. NM Technologies" */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <string>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_stdlib.h"

/*--------------------------------------------------/
/                   Globals                         /
/--------------------------------------------------*/

using namespace std;
class HCxSR04
{
  public:
    HCxSR04(string hw_name, int trigger, int echo);
    double distanceMeters;
    double distance();

  private:
    void init_hw();
    int trigger;
    int echo;
    string hw_name;
    bool sim_mode;
    volatile long startTimeUsec;
    volatile long endTimeUsec;
    long travelTimeUsec;
};

#endif
