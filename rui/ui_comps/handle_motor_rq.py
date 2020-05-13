#!/usr/bin/env python

"""
"  @file      handle_motor_rq.py
"  @brief     Handler for Motor Action Requests
"  @details   Handle Motor actions and send requests
"  @author    Nitin Mohan
"  @date      May 13, 2020
"  @copyright 2020 - NM Technologies
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#
from ui_comps.LayoutBase import *

#---------------------------------------------------#
#                   Global Variables                #
#---------------------------------------------------#
HOME_ANGLE = 90.00
DEFAULT_SPEED = 50

class MotorControlHandle(LayoutBase):

    def motor_ctrl_action(self, motor, direction="", angle="", speed=""):

        """ 
        "  @brief Validate Button Press and Perform Action
        "  @param[in] motor
        "  @param[in] direction
        """

        #-- Check Preconditions --#
        if (motor == "CAM_HRZN_MTR" or motor == "CAM_VERT_MTR") \
                and not angle:

           self.throw_error("Please Provide a Angle!")

        elif (motor == "CAM_HRZN_MTR" or motor == "CAM_VERT_MTR") \
                and angle:

            try:
                angle = int(angle)
            except ValueError:
                self.throw_error("Invalid Angle Provided!")
        else:
             angle = -1

        if speed != "":
            try:
                speed = int(speed)
            except ValueError:
                self.throw_error("Invalid Speed Provided!")
        else:
            speed = -1

        if motor == "DRIVE_DRV_MTR":

            #-- Move the motor --#
            self.__drive(direction, speed)

        else:

            #-- Move the motor --#
            self.__move_motor(motor, direction, angle=angle, speed=speed)

    def __drive(self, direction, speed):

        """ 
        "  @brief Drive the Robot Forward/Reverse
        """

        self.__move_motor("LEFT_DRV_MTR", direction=direction, speed=speed)
        self.__move_motor("RIGHT_DRV_MTR", direction=direction, speed=speed)

    def drive_left(self):

        """ 
        "  @brief Drive the Robot Left
        """

        self.__move_motor("LEFT_DRV_MTR", direction="REVERSE", speed=DEFAULT_SPEED)
        self.__move_motor("RIGHT_DRV_MTR", direction="FORWARD", speed=DEFAULT_SPEED)


    def drive_right(self):

        """ 
        "  @brief Drive the Robot Right
        """

        self.__move_motor("LEFT_DRV_MTR", direction="FORWARD", speed=DEFAULT_SPEED)
        self.__move_motor("RIGHT_DRV_MTR", direction="REVERSE", speed=DEFAULT_SPEED)

    def move_camera_home(self):

        """ 
        "  @brief Move the Camera Motors to Home Position
        """

        self.__move_motor("CAM_HRZN_MTR", angle=HOME_ANGLE)
        self.__move_motor("CAM_VERT_MTR", angle=HOME_ANGLE)

    def __move_motor(self, motor, direction="", angle=-1, speed=-1):

        #-- Move the Motor --#
        try:
            self.nibot_ap.perform_nibot_motor_action(motor, direction=direction, angle=angle, speed=speed)
        except Exception as e:
            print (e)
            self.throw_error("Failed to perform motor action!")
