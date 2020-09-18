#!/usr/bin/env python

"""
"  @file      DriveMotorControlBox.py
"  @brief     UI for Drive Motor Control Box
"  @details   All Components for Drive Motor
"  @author    Nitin Mohan
"  @date      April 26, 2020
"  @copyright 2020 - NM Technologies
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
import tkinter as tk
import tkinter.ttk as ttk

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#
from ui_comps.handle_motor_rq import MotorControlHandle
from ui_comps.LayoutBase import *
from lib.global_var import *

""" 
"  @class  DriveMotorControlBox
"          Drive Motor Control Object
"""
class DriveMotorControlBox(MotorControlHandle):

    def _class_vars_init(self):

        """ 
        "  @brief Initialize Class Variables
        """

        self.motor = tk.StringVar()

        self.FORWARD = lambda: self.motor_ctrl_action(self.motor.get(),
                                                      direction="FORWARD",
                                                      speed=self.speed_txbx.get())
        self.REVERSE = lambda: self.motor_ctrl_action(self.motor.get(),
                                                      direction="REVERSE",
                                                      speed=self.speed_txbx.get())
        self.STOP    = lambda: self.motor_ctrl_action(self.motor.get(),
                                                      "STOP")
        self.LEFT    = lambda: self.drive_left()
        self.RIGHT   = lambda: self.drive_right()

    def _class_comps_init(self):

        """ 
        "  @brief Initialize Class Components
        """

        self.motor.set("DRIVE_DRV_MTR")

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
                                           value="DRIVE_DRV_MTR")

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
    
