#!/usr/bin/env python

"""
"  @file      main.py
"  @brief     Main Entry to launch the Robot User Interface
"  @details   Main entry to launch the user-interface
"  @author    Nitin Mohan
"  @date      April 11, 2020
"  @copyright 2020 - NM Technologies
"""
#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
from tkinter import *
from tkinter.ttk import *

#---------------------------------------------------#
#                   Constants                       #
#---------------------------------------------------#

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#


class Layout(object):
    def __init__(self, window):
        self.window = window
        self.__grpbox()
        self.__images()
        self.__buttons()
        self.__inputs()
        self.__layout()

    def __buttons(self):
        self.cbtn = Button(self.window, text="Connect")
        self.dcbtn = Button(self.window, text="Disconnect")
        self.upbtn = Button(self.camctrl_gb, text="UP")
        self.downbtn = Button(self.camctrl_gb, text="DOWN")
        self.leftbtn = Button(self.camctrl_gb, text="LEFT")
        self.rightbtn = Button(self.camctrl_gb, text="RIGHT")

    def __inputs(self):
        self.host_txtbox = Entry(self.window)

    def __grpbox(self):
        self.camctrl_gb = LabelFrame(self.window, text="Camera Control")

    def __images(self):
        self.logo = Canvas(self.window) 

    def __layout(self):
        self.window.title("NiBot")
        self.window.geometry('810x610')
        self.logo.place(x=0, y=0, img="imgs/nibot.png")
        self.camctrl_gb.place(x=50, y=100, height=100, width=310)
        self.host_txtbox.place(x=500, y=10, height=30, width=100)
        self.cbtn.place(x=600, y=10, height=30, width=100)
        self.dcbtn.place(x=700, y=10, height=30, width=100)
        self.upbtn.place(x=100, y=0, height=30, width=100)
        self.downbtn.place(x=100, y=30, height=30, width=100)
        self.leftbtn.place(x=0, y=30, height=30, width=100)
        self.rightbtn.place(x=200, y=30, height=30, width=100)


if __name__ == '__main__':
    window = Tk()
    Layout(window)
    window.mainloop()
