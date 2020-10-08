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
DRIVE_MOTORS = ["DRIVE_DRV_MTR", "LEFT_DRV_MTR", "RIGHT_DRV_MTR"]

class MotorControlHandle(LayoutBase):

    def __init__(self, window, nibot_ap):
        super().__init__(window, nibot_ap)
        self.home_angle = self.nibot_ap.rui_settings['motor_defaults']['home_angle']
        self.default_turn_speed = self.nibot_ap.rui_settings['motor_defaults']['default_turn_speed']

    def motor_ctrl_action(self, motor, direction="", angle="", speed=""):

        """ 
        "  @brief Validate Button Press and Perform Action
        "  @param[in] motor
        "  @param[in] direction
        "  @param[in] angle
        "  @param[in] speed
        """

        # -- Check for Single Camera Motor Movement -- #
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

        # -- Check if Speed is provided --#
        if speed != "" and motor in DRIVE_MOTORS:
            try:
                speed = int(speed)
            except ValueError:
                self.throw_error("Invalid Speed Provided!")
        else:
            speed = -1

        #-- Check for Dual Drive Motor Movement --#
        if motor == "DRIVE_DRV_MTR":

            #-- Move the motor --#
            self.__drive(direction, speed)

        else:

            #-- Construct Action from Args Passed --#
            action = [(motor, direction, angle, speed)]

            #-- Move the motor --#
            self.__move_motor(action)

    def __drive(self, direction, speed):

        """ 
        "  @brief Drive the Robot Forward/Reverse
        """

        #-- Construct Actions --#
        action =[("LEFT_DRV_MTR", direction, -1, speed),
                 ("RIGHT_DRV_MTR", direction, -1, speed)]

        #-- Perform Action --#
        self.__move_motor(action)

    def drive_left(self, speed=None):

        """ 
        "  @brief Drive the Robot Left
        """

        # -- Check Speed --#
        if not speed:
            speed = self.default_turn_speed

        #-- Construct Actions --#
        action = [("LEFT_DRV_MTR", "REVERSE", -1, speed),
                 ("RIGHT_DRV_MTR", "FORWARD", -1, speed)]

        #-- Perform Action --#
        self.__move_motor(action)


    def drive_right(self, speed=None):

        """ 
        "  @brief Drive the Robot Right
        """

        # -- Check Speed --#
        if not speed:
            speed = self.default_turn_speed

        #-- Construct Actions --#
        action = [("LEFT_DRV_MTR", "FORWARD", -1, speed),
                 ("RIGHT_DRV_MTR", "REVERSE", -1, speed)]

        #-- Perform Action --#
        self.__move_motor(action)

    def move_camera_home(self):

        """ 
        "  @brief Move the Camera Motors to Home Position
        """

        # -- Construct Actions --#
        action = [("CAM_HRZN_MTR", "",  self.home_angle, -1),
                 ("CAM_VERT_MTR",  "",  self.home_angle, -1)]

        #-- Perform Action --#
        self.__move_motor(action)

    def __move_motor(self, action):

        """ 
        "  @brief Send Request to Perform Action on NiBot
        "  @param[in] action
        """

        try:
            #-- Move the Motor --#
            self.nibot_ap.perform_nibot_motor_action(action)
        except Exception as e:
            print (e)
            self.throw_error("Failed to perform motor action!")
