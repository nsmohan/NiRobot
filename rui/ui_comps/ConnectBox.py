#!/usr/bin/env python

"""
"  @file      ConnectBox.py
"  @brief     Connect-Box-Widget
"  @details   Object defniton for components required to
"             connect to the robot
"  @author    Nitin Mohan
"  @date      April 23, 2020
"  @copyright 2020 - NM Technologies
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
import tkinter as tk
import tkinter.ttk as ttk
import json

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#
from ui_comps.LayoutBase import *

""" 
"  @class  ConnectBox
"          Object for Robot Connect/Disconnect
"""
class ConnectBox(LayoutBase):

    def __init__(self, window):

        """ 
        "  @brief Constructor for ConnectBox
        "  @param[in] window -> Parent Element
        """

        # -- Initialize Class -- #
        super().__init__()
        self.window = window
        self.__grpbox()
        self.__buttons()
        self.__inputs()
        self.__layout()
        
    def __grpbox(self):
        
        """ 
        "  @brief Group Boxes defined for element
        """

        self.connect_gb = tk.LabelFrame(self.window, text="Connect-to-NiBot", bg=self.std_bg_color)

    def __buttons(self):

        """ 
        "  @brief Buttons defined for element
        """

        self.cbtn =  self.new_button(self.connect_gb, "Connect")
        self.dcbtn = self.new_button(self.connect_gb, "Disconnect")

    def __inputs(self):

        """ 
        "  @brief Inputs for element
        """

        self.host_txtbox = ttk.Entry(self.connect_gb)

    def __layout(self):

        """ 
        "  @brief Element Layout
        """

        box_width = (self.std_button_width * 3) + 20
        box_height = (self.std_button_height) + 30
        self.connect_gb.place(x=MAX_WIDTH - box_width, y=0, height=box_height, width=box_width)
        self.host_txtbox.place(x=0, y=box_height/10, width=self.std_button_width, height=self.std_button_height)
        self.cbtn.place(x=self.std_button_width, y=box_height/10)
        self.dcbtn.place(x=self.std_button_width*2, y=box_height/10)
