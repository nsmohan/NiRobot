#!/usr/bin/env python

"""
"  @file      global_var.py
"  @brief     Library for setting/retriving global Variables used by the program
"  @details   Global Varible Handler
"  @author    Nitin Mohan
"  @date      September 18, 2020
"  @copyright 2020
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
import screeninfo

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#

#---------------------------------------------------#
#                   Global Varibles                 #
#---------------------------------------------------#
HEADER_HEIGHT = 200
BODY_HEIGHT = 600
MIN_X = 10
MIN_Y = 10
MIN_FRAME = 20
WIDTH_ADJUST_FACTOR = 500
HEIGHT_ADJUST_FACTOR = 300

#--------------------------------------------------------------------------#
#                               Start of Program                           #
#--------------------------------------------------------------------------#
def update_screen_resolution_vars(window):
    """ 
    "  @brief Get Current Screen Resolutions
    """

    global MaxWidth
    global MaxHeight
    global ScreenWidth
    global ScreenHeight

    xpos = window.winfo_x()
    ypos = window.winfo_y()

    current_monitor = get_current_monitor(xpos, ypos)

    ScreenHeight = current_monitor.height - HEIGHT_ADJUST_FACTOR
    ScreenWidth  = current_monitor.width - WIDTH_ADJUST_FACTOR

    MaxHeight = ScreenHeight - MIN_FRAME
    MaxWidth  = ScreenWidth - MIN_FRAME

def get_current_monitor(xpos, ypos):
    """ 
    "  @brief Get current screen application is on
    """
    monitors = screeninfo.get_monitors()

    for m in reversed(monitors):
        if m.x <= xpos <= m.width + m.x and m.y <= ypos <= m.height + m.y:
            return m
    return monitors[0]



