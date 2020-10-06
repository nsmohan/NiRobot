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

        """ 
        "  @brief Initialize Class
        """

        self.default_button_states()
        self.__update_hosts_box()

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

        self.host_txtbox = ttk.Combobox(self.connect_gb)

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

    def __update_hosts_box(self):

        """ 
        "  @brief Update Combox List
        """

        for host in self.__get_known_hosts():
            self.host_txtbox['value'] += host

    def __handle_connect(self):

        """ 
        "  @brief Handle Connect Button
        """

        hostname = self.host_txtbox.get()
        try:
            self.nibot_ap.connect_to_nibot(hostname)
        except Exception as e:
            self.throw_error("Unable to Connect to NiBot! {}".format(e))
        else:
            self.active_button_states()
            if hostname not in self.host_txtbox['values']:
                self.host_txtbox['values'] += hostname


    def __handle_disconnect(self):

        """ 
        "  @brief Handle Disconnect Button
        """

        self.nibot_ap.disconnect_from_nibot()
        self.default_button_states()

    def __get_known_hosts(self):

        """ 
        "  @brief Get list of known hosts
        """

        return self.nibot_ap.rui_settings["known_host"]


    def default_button_states(self):

        """ 
        "  @brief Set default button states
        """

        self.cbtn["state"] = "normal"
        self.dcbtn["state"] = "disabled"

    def active_button_states(self):

        """ 
        "  @brief Set active button states
        """

        self.cbtn["state"] = "disabled"
        self.dcbtn["state"] = "normal"
