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
    def __init__(self, window):

        """ 
        "  @brief Constructor
        "  param[in] window
        """

        super().__init__()
        self.window = window
        self.__grpbox()
        self.__inputs()
        self.__labels()
        self.__buttons()
        self.__layout()

    def __inputs(self):

        """ 
        "  @brief Input Boxes
        """

        self.angle_txbx = tk.Entry(self.camctrl_gb)

    def __labels(self):

        """ 
        "  @brief Label Definitions
        """

        self.angle_label = tk.Label(self.camctrl_gb, text="Enter Angle:")

    def __grpbox(self):

        """ 
        "  @brief Group Box Definitions
        """

        self.camctrl_gb = self.new_label_frame(self.window, text="Camera Control")

    def __buttons(self):

        """ 
        "  @brief Button Definitions
        """

        self.upbtn    = self.new_button(self.camctrl_gb, "UP")
        self.downbtn  = self.new_button(self.camctrl_gb, "DOWN")
        self.leftbtn  = self.new_button(self.camctrl_gb, "LEFT")
        self.rightbtn = self.new_button(self.camctrl_gb, "RIGHT")
        self.homebtn  = self.new_button(self.camctrl_gb, "HOME")
        self.custbtn  = self.new_button(self.camctrl_gb, "CUSTOM")

    def __layout(self):

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
        self.custbtn.place(x=300, y=120)


""" 
"  @class  DriveMotorControlBox
"          Drive Motor Control Object
"""
class DriveMotorControlBox(LayoutBase):
    def __init__(self, window):

        """ 
        "  @brief Constructor
        """

        super().__init__()
        self.window = window
        self.direction = tk.StringVar()
        self.__grpbox()
        self.__inputs()
        self.__radiobuttons()
        self.__labels()
        self.__buttons()
        self.__layout()

    def __inputs(self):

        """ 
        "  @brief Inputs
        """

        self.speed_txbx = tk.Entry(self.drive_ctrl_gb)

    def __radiobuttons(self):

        """ 
        "  @brief Radio Buttons
        """

        self.left_drv_rad = tk.Radiobutton(self.drive_ctrl_gb, text="Left Motor", variable=self.direction, value="LEFT_DRV_MTR")
        self.right_drv_rad = tk.Radiobutton(self.drive_ctrl_gb, text="Right Motor", variable=self.direction, value="RIGHT_DRV_MTR")
        self.both_drv_rad = tk.Radiobutton(self.drive_ctrl_gb, text="Both",   variable=self.direction, value="BOTH_DRV_MOTORS")

    def __labels(self):

        """ 
        "  @brief Labels
        """

        self.speed_label = tk.Label(self.drive_ctrl_gb, text="Speed:")

    def __grpbox(self):

        """ 
        "  @brief Group Box
        """

        self.drive_ctrl_gb = self.new_label_frame(self.window, text="Drive Motor Control")

    def __buttons(self):

        """ 
        "  @brief Button Definitions
        """

        self.fwdbtn    = self.new_button(self.drive_ctrl_gb, "FORWARD")
        self.stopbtn  = self.new_button(self.drive_ctrl_gb, text="STOP")
        self.revbtn  = self.new_button(self.drive_ctrl_gb, text="REVERSE")
        self.leftbtn  = self.new_button(self.drive_ctrl_gb, text="LEFT")
        self.rightbtn  = self.new_button(self.drive_ctrl_gb, text="RIGHT")

    def __layout(self):

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
