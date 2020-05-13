#!/usr/bin/env python

"""
"  @file      CameraControlBox.py
"  @brief     UI Layout for Camera Control Box
"  @details   Components for Camera Control Box
"  @author    Nitin Mohan
"  @date      May 13, 2020
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

""" 
"  @class  CameraControlBox
"          Camera Control Box Object
"""
class CameraControlBox(MotorControlHandle):

    def _class_vars_init(self):

        """ 
        "  @brief Initialize Class Variables
        """

        self.UP       = lambda: self.motor_ctrl_action("CAMERA", "UP")
        self.DOWN     = lambda: self.motor_ctrl_action("CAMERA", "DOWN")
        self.LEFT     = lambda: self.motor_ctrl_action("CAMERA", "LEFT")
        self.RIGHT    = lambda: self.motor_ctrl_action("CAMERA", "RIGHT")
        self.HOME     = lambda: self.move_camera_home()
        self.HRNZ_MTR = lambda: self.motor_ctrl_action("CAM_HRZN_MTR", angle=self.angle_txbx.get())
        self.VERT_MTR = lambda: self.motor_ctrl_action("CAM_VERT_MTR", angle=self.angle_txbx.get())

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

        self.upbtn    = self.new_button(self.camctrl_gb, "UP",         command=self.UP)
        self.downbtn  = self.new_button(self.camctrl_gb, "DOWN",       command=self.DOWN)
        self.leftbtn  = self.new_button(self.camctrl_gb, "LEFT",       command=self.LEFT)
        self.rightbtn = self.new_button(self.camctrl_gb, "RIGHT",      command=self.RIGHT)
        self.homebtn  = self.new_button(self.camctrl_gb, "HOME",       command=self.HOME)
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
