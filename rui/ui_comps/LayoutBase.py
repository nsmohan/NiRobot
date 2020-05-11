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
BODY_HEIGHT = 600
MIN_X = 10
MIN_Y = 10

#---------------------------------------------------#
#                 Start of Program                  #
#---------------------------------------------------#

""" 
"  @class  LayoutBase
"          Skeleon Object for Layout
"""
class LayoutBase(object):

    def __init__(self, window, nibot_ap):

        """ 
        "  @brief Constructor
        """

        # -- Global Class Variables --#
        self.window = window
        self.nibot_ap = nibot_ap

        # -- Initialize Variables --#
        self.style = ttk.Style()
        self.std_button_width  = 150
        self.std_button_height = 30
        self.L_button_width    = 300
        self.L_button_height   = 50
        self.lframe_height     = 200
        self.lframe_width      = 450
        self.std_bg_color      = "#F3F3F6"

        #-- Initialize Class --#
        self.__styles()
        self._class_vars_init()
        self._tabs()
        self._frames()
        self._grpbox()
        self._buttons()
        self._radiobuttons()
        self._tree()
        self._inputs()
        self._images()
        self._labels()
        self._class_comps_init()
        self._layout()
        
    #---------------------------------------------------#
    #                Abstract Definitions               #
    #---------------------------------------------------#
    def _tabs(self):
        pass

    def _frames(self):
        pass

    def _grpbox(self):
        pass

    def _buttons(self):
        pass

    def _radiobuttons(self):
        pass

    def _tree(self):
        pass

    def _inputs(self):
        pass

    def _labels(self):
        pass

    def _images(self):
        pass

    def _layout(self):
        pass

    def _class_vars_init(self):
        pass

    def _class_comps_init(self):
        pass

    #---------------------------------------------------#
    #                Private Methods                    #
    #---------------------------------------------------#
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

    #---------------------------------------------------#
    #                Public Methods                     #
    #---------------------------------------------------#
    def new_button(self, window, text, style="std", command=None):

        """ 
        "  @brief Wrapper method to create a new button
        """

        return ttk.Button(window, text=text, style=f"{style}.TButton", command=command)

    def new_label_frame(self, window, text):
            return ttk.LabelFrame(self.window, text=text, width=self.lframe_width, height=self.lframe_height)

    @staticmethod
    def throw_error(error):

        """ 
        "  @brief Display Error Messaged Passed
        "  @param[in] error
        """

        tk.messagebox.showerror(title="NiBot", message=error)

