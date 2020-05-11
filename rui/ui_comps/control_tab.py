#!/usr/bin/env python

"""
"  @file      control_tab.py
"  @brief     User Layout for Control Tab
"  @details   UI Components for Control Tab
"  @author    Nitin Mohan
"  @date      April 26, 2020
"  @copyright 2020 - NM Technologies
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
import tkinter as tk
import tkinter.ttk as ttk
import PIL

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#
from ui_comps.LayoutBase import *

""" 
"  @class  CameraControlBox
"          Camera Control Box Object
"""
class CameraControlBox(LayoutBase):

    def _class_vars_init(self):

        """ 
        "  @brief Initialize Class Variables
        """

        self.UP       = lambda: self.__handle_camera_button("CAMERA", "UP")
        self.DOWN     = lambda: self.__handle_camera_button("CAMERA", "DOWN")
        self.LEFT     = lambda: self.__handle_camera_button("CAMERA", "LEFT")
        self.RIGHT    = lambda: self.__handle_camera_button("CAMERA", "RIGHT")
        self.HOME     = lambda: self.__handle_camera_button("CAMERA", "HOME")
        self.HRNZ_MTR = lambda: self.__handle_camera_button("CAM_HRZN_MTR")
        self.VERT_MTR = lambda: self.__handle_camera_button("CAM_VERT_MTR")

    def _inputs(self):

        """ 
        "  @brief Input Boxes
        """

        self.angle_txbx = tk.Entry(self.camctrl_gb)

    def _labels(self):

        """ 
        "  @brief Label Definitions
        """

        self.angle_label = tk.Label(self.camctrl_gb, text="Enter Angle:")

    def _grpbox(self):

        """ 
        "  @brief Group Box Definitions
        """

        self.camctrl_gb = self.new_label_frame(self.window, text="Camera Control")

    def _buttons(self):

        """ 
        "  @brief Button Definitions
        """

        self.upbtn    = self.new_button(self.camctrl_gb, "UP",     command=self.UP)
        self.downbtn  = self.new_button(self.camctrl_gb, "DOWN",   command=self.DOWN)
        self.leftbtn  = self.new_button(self.camctrl_gb, "LEFT",   command=self.LEFT)
        self.rightbtn = self.new_button(self.camctrl_gb, "RIGHT",  command=self.RIGHT)
        self.homebtn  = self.new_button(self.camctrl_gb, "HOME",   command=self.HOME)
        self.hrznbtn  = self.new_button(self.camctrl_gb, "HRZN MOTOR", command=self.HRNZ_MTR)
        self.vertbtn  = self.new_button(self.camctrl_gb, "VERT MOTOR", command=self.VERT_MTR)

    def _layout(self):

        """ 
        "  @brief Layout of Components
        """

        self.upbtn.place(x=150, y=0)
        self.homebtn.place(x=150, y=30)
        self.downbtn.place(x=150, y=60)
        self.leftbtn.place(x=0, y=30)
        self.rightbtn.place(x=300, y=30)
        self.angle_label.place(x=0, y=120, height=self.std_button_height, width=self.std_button_width)
        self.angle_txbx.place(x=150, y=120, height=self.std_button_height, width=self.std_button_width)
        self.hrznbtn.place(x=300, y=60)
        self.vertbtn.place(x=0, y=60)


    def __handle_camera_button(self, motor, direction=""):

        """ 
        "  @brief Handle for Camera direction buttons
        "  @param[in] direction
        """

        #-- Initialize Variables --#
        angle = -1

        #-- Check Preconditions --#
        if motor == "CAM_HRZN_MTR" or motor == "CAM_VERT_MTR":
            try:
                angle = int(self.angle_txbx.get())
            except ValueError:
                self.throw_error("Invalid Angle Provided!")

        #-- Move the Motor --#
        try:
            self.nibot_ap.perform_nibot_motor_action(motor, direction, angle=angle)
        except Exception as e:
            print (e)
            self.throw_error("Failed to perform motor action!")

""" 
"  @class  DriveMotorControlBox
"          Drive Motor Control Object
"""
class DriveMotorControlBox(LayoutBase):

    def _class_vars_init(self):

        """ 
        "  @brief Initialize Class Variables
        """

        self.motor = tk.StringVar()

        self.FORWARD = lambda: self.__handle_drv_button("FORWARD")
        self.REVERSE = lambda: self.__handle_drv_button("REVERSE")
        self.STOP    = lambda: self.__handle_drv_button("STOP")
        self.LEFT    = lambda: self.__handle_drv_button("LEFT")
        self.RIGHT   = lambda: self.__handle_drv_button("RIGHT")

    def _class_comps_init(self):

        """ 
        "  @brief Initialize Class Components
        """

        self.motor.set("DRIVE")

    def _inputs(self):

        """ 
        "  @brief Inputs
        """

        self.speed_txbx = tk.Entry(self.drive_ctrl_gb)

    def _radiobuttons(self):

        """ 
        "  @brief Radio Buttons
        """

        # -- Left motor Radio Button --#
        self.left_drv_rad = tk.Radiobutton(self.drive_ctrl_gb,
                                           text="Left Motor",
                                           variable=self.motor,
                                           value="LEFT_DRV_MTR")

        #-- Right Motor Radio Button --#
        self.right_drv_rad = tk.Radiobutton(self.drive_ctrl_gb,
                                            text="Right Motor",
                                            variable=self.motor,
                                            value="RIGHT_DRV_MTR")

        #-- Both Motors Radio Button --#
        self.both_drv_rad = tk.Radiobutton(self.drive_ctrl_gb,
                                           text="Both",
                                           variable=self.motor,
                                           value="DRIVE")

    def _labels(self):

        """ 
        "  @brief Labels
        """

        self.speed_label = tk.Label(self.drive_ctrl_gb, text="Speed:")

    def _grpbox(self):

        """ 
        "  @brief Group Box
        """

        self.drive_ctrl_gb = self.new_label_frame(self.window, text="Drive Motor Control")

    def _buttons(self):

        """ 
        "  @brief Button Definitions
        """

        self.fwdbtn   = self.new_button(self.drive_ctrl_gb, text="FORWARD", command=self.FORWARD)
        self.stopbtn  = self.new_button(self.drive_ctrl_gb, text="STOP",    command=self.STOP)
        self.revbtn   = self.new_button(self.drive_ctrl_gb, text="REVERSE", command=self.REVERSE)
        self.leftbtn  = self.new_button(self.drive_ctrl_gb, text="LEFT",    command=self.LEFT)
        self.rightbtn = self.new_button(self.drive_ctrl_gb, text="RIGHT",   command=self.RIGHT)

    def _layout(self):

        """ 
        "  @brief Layout of Components
        """

        self.fwdbtn.place(x=150, y=0)
        self.stopbtn.place(x=150, y=30)
        self.revbtn.place(x=150, y=60)
        self.leftbtn.place(x=0, y=30)
        self.rightbtn.place(x=300, y=30)
        self.speed_label.place(x=0, y=120)
        self.speed_label.place(x=0, y=120, height=self.std_button_height, width=self.std_button_width)
        self.speed_txbx.place(x=150, y=120, height=self.std_button_height, width=self.std_button_width)
        self.drive_ctrl_gb.place(x=MIN_X, y=MIN_Y + 150)
        self.left_drv_rad.place(x=295, y=90)
        self.right_drv_rad.place(x=295, y=120)
        self.both_drv_rad.place(x=295, y=150)

    def __handle_drv_button(self, direction):

        # -- Initialize Variables --#
        speed = -1

        #-- Check Preconditions --#
        if self.speed_txbx.get() != "":
            try:
                speed = int(self.angle_txbx.get())
            except ValueError:
                self.throw_error("Invalid Speed Provided!")

        #-- Move the Motor --#
        try:
            self.nibot_ap.perform_nibot_motor_action(self.motor.get(), direction, speed=speed)
        except Exception as e:
            print (e)
            self.throw_error("Failed to perform motor action!")

    
