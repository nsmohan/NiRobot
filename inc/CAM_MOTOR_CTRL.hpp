#ifndef DEF_CAM_MOTOR_CTRL
#define DEF_CAM_MOTOR_CTRL

/*CAM_MOTOR_CTRL.hpp: Header File for Camera Control Interface

__author__      = "Nitin Mohan
__copyright__   = "Copy Right 2019. NM Technologies" */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <iostream>
#include <string>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_stdlib.h"

/*--------------------------------------------------/
/                   Globals                         /
/--------------------------------------------------*/
using namespace std;
const int MOTOR_SENSITIVITY = 10; //Default Angle the motor moves
extern const string DIRECTION_TO_STR[5] = {"UP", "DOWN", 
                                           "LEFT", "RIGHT", 
                                           "CUSTOM"};


/*--------------------------------------------------/
/                   Strucs/Classes/Enums            /
/--------------------------------------------------*/
typedef enum {UP, DOWN, 
              LEFT, RIGHT, 
              CUSTOM} CAM_MOTOR_CTRL_DIRECTIONS;

class Camera_Motor_Ctrl
{
    public:
        string horizontal_motor;
        string vertical_motor;
        Camera_Motor_Ctrl();

        /* In - Direction to move
         * In - Optional Name of the motor to move
         * In - Optional Custom angle to move
         * In - Optional Motor Sensitivity */
        NMT_result CAM_MTR_CTRL_MOVE_CAMERA(CAM_MOTOR_CTRL_DIRECTIONS direction, 
                                            string motor = "", 
                                            double angle_to_move = 0.00, 
                                            double default_angle = MOTOR_SENSITIVITY);
};
#endif
