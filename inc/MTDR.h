/*MTDR.h:         <Comment>

__author__      = "Nitin Mohan
__copyright__   = "Copy Right 2019. NM Technologies" */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <stdlib.h>
/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_log.h"
#include "PCA9685.h"

/*--------------------------------------------------/
/                   Constants                       /
/--------------------------------------------------*/

/*--------------------------------------------------/
/                   Structures                      /
/--------------------------------------------------*/
const struct
{
    PCA9685_PWM_CHANNEL channel;
    const char          *motor;
} MTDR_motors [] = 
{
    {CHANNEL_15, "CAM_HRZN_MTR"},
    {CHANNEL_14, "CAM_VERT_MTR"}
};

//------------------Prototypes----------------------//
NMT_result MTDR_move_motor(char *motor, double angle, PCA9685_settings *settings);
NMT_result MTDR_get_current_position(char *motor, double *angle, PCA9685_settings *settings);
NMT_result MTDR_get_pca9685_status(PCA9685_settings *settings, bool *initialized);
NMT_result MTDR_init(PCA9685_settings *settings);

