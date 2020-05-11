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


        
    def _class_comps_init(self):
        self.default_button_states()

    def _grpbox(self):
        
        """ 
        "  @brief Group Boxes defined for element
        """

        self.connect_gb = tk.LabelFrame(self.window, text="Connect-to-NiBot", bg=self.std_bg_color)

    def _buttons(self):

        """ 
        "  @brief Buttons defined for element
        """

        self.cbtn =  self.new_button(self.connect_gb, "Connect", command=self.__handle_connect)
        self.dcbtn = self.new_button(self.connect_gb, "Disconnect", command=self.__handle_disconnect)

    def _inputs(self):

        """ 
        "  @brief Inputs for element
        """

        self.host_txtbox = ttk.Entry(self.connect_gb)

    def _layout(self):

        """ 
        "  @brief Element Layout
        """

        box_width = (self.std_button_width * 3) + 20
        box_height = (self.std_button_height) + 30
        self.connect_gb.place(x=MAX_WIDTH - box_width, y=0, height=box_height, width=box_width)
        self.host_txtbox.place(x=0, y=box_height/10, width=self.std_button_width, height=self.std_button_height)
        self.cbtn.place(x=self.std_button_width, y=box_height/10)
        self.dcbtn.place(x=self.std_button_width*2, y=box_height/10)

    def __handle_connect(self):

        try:
            self.nibot_ap.connect_to_nibot(self.host_txtbox.get())
            self.active_button_states()
        except Exception as e:
            self.throw_error("Unable to Connect to NiBot!")

    def __handle_disconnect(self):
        self.nibot_ap.disconnect_from_nibot()
        self.default_button_states()

    def default_button_states(self):
        self.cbtn["state"] = "normal"
        self.dcbtn["state"] = "disabled"

    def active_button_states(self):
        self.cbtn["state"] = "disabled"
        self.dcbtn["state"] = "normal"


