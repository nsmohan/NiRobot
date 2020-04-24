#!/usr/bin/env python

"""
"  @file      LayoutBase.py
"  @brief     Base Class for User-Interface Layout
"  @details   Base Class for User-Interface Layout
"  @author    Nitin Mohan
"  @date      April 23, 2020
"  @copyright 2020 - NM Technologies
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
import tkinter as tk
import tkinter.ttk as ttk

#---------------------------------------------------#
#                   Constants                       #
#---------------------------------------------------#
WIDTH  = "1600"
HEIGHT = "900"
MAX_WIDTH = 1580
MAX_HEIGHT = 880
HEADER_HEIGHT = 200
BODY_HEIGHT = 500

#---------------------------------------------------#
#                 Start of Program                  #
#---------------------------------------------------#

""" 
"  @class  LayoutBase
"          Skeleon Object for Layout
"""
class LayoutBase(object):

    def __init__(self):

        """ 
        "  @brief Constructor
        """

        self.style = ttk.Style()
        self.__styles()
        self.std_button_width = 150
        self.std_button_height = 30
        self.L_button_size = 300
        self.std_bg_color = "#F3F3F6"

    def __styles(self):

        """ 
        "  @brief Layout Style Definitions
        """

        self.style.configure('std.TButton',
                             font = ('comic', 10, 'bold'), 
                             width=15) 
        self.style.configure('L.TButton',
                             font = ('comic', 20, 'bold'), 
                             width=15) 

    def new_button(self, window, text, style="std"):

        """ 
        "  @brief Wrapper method to create a new button
        """

        return ttk.Button(window, text=text, style=f"{style}.TButton")
