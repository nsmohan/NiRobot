/*.h:         <Comment>

__author__      = "Nitin Mohan
__copyright__   = "Copy Right 2019. NM Technologies" */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <stdbool.h>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_stdlib.h"

/*--------------------------------------------------/
/                   Constants                       /
/--------------------------------------------------*/

/*------------------Prototypes----------------------*/
extern NMT_result RSXA_get_mode(char *hw_name,     //Input - Name of hardware to look for
                              bool *sim_mode);    //Output - True if in simulation else false
