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
import tkinter as tk
import tkinter.ttk as ttk
import PIL
from PIL import ImageTk
from PIL import Image
import os

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#
from ui_comps.SettingsView import SettingsTabView
from ui_comps.LayoutBase import *
from ui_comps.ConnectBox import ConnectBox
from ui_comps.control_tab import CameraControlBox
from ui_comps.control_tab import DriveMotorControlBox


#---------------------------------------------------#
#                   Constants                       #
#---------------------------------------------------#
WINDOW_TITLE = "NiBot"
MY_DIR = os.path.join(os.getcwd().split("NiRobot")[0], "NiRobot")
RUI_DIR = os.path.join(MY_DIR, "rui")
IMG_DIR = os.path.join(RUI_DIR, "imgs")


class LayoutHeader(LayoutBase):
    def __init__(self, window):
        self.window = window
        self.__images()
        self.__layout()
        ConnectBox(self.window)

    def __images(self):
        load = Image.open(os.path.join(IMG_DIR, "nibot.png"))
        render = ImageTk.PhotoImage(load)
        self.logo = tk.Label(self.window, image=render)
        self.logo.image = render

    def __layout(self):
        self.logo.place(x=0, y=0)


class LayoutBody(object):
    def __init__(self, window):
        self.window = window
        self.settings_tab_width = 700
        self.__tabs()
        self.__Frames()
        self.cam_ctrl = CameraControlBox(self.control_tab)
        self.drive_ctrl = DriveMotorControlBox(self.control_tab)
        self.settings_view = SettingsTabView(self.rsxa_settings_box)
        self.__layout()

    def __Frames(self):
        self.rsxa_settings_box = tk.Frame(self.settings_tab, height=BODY_HEIGHT, width=self.settings_tab_width)

    def __tabs(self):
        self.bodytab = ttk.Notebook(self.window)
        self.settings_tab = ttk.Frame(self.bodytab)
        self.control_tab = ttk.Frame(self.bodytab)
        self.bodytab.add(self.control_tab, text="Control")
        self.bodytab.add(self.settings_tab, text="Settings")

    def __layout(self):
        self.bodytab.place(x=0, y=0, height=BODY_HEIGHT, width=MAX_WIDTH - 10)
        self.cam_ctrl.camctrl_gb.place(x=MIN_X, y=MIN_Y)
        self.rsxa_settings_box.place(x=MIN_X, y=MIN_Y)
        self.drive_ctrl.drive_ctrl_gb.place(x=MIN_X, y=MIN_Y + 200)
        
class GUIController(LayoutBase):
    def __init__(self, window):
        super().__init__()
        self.window = window
        window.configure(bg=self.std_bg_color)
        self.__Frames()
        self.__layout()
        self.header = LayoutHeader(self.header_box)
        self.body = LayoutBody(self.body_box)

    def __Frames(self):
        self.border_box = tk.Frame(self.window, height=MAX_HEIGHT, width=MAX_WIDTH, bg=self.std_bg_color)
        self.header_box = tk.Frame(self.border_box, height = HEADER_HEIGHT, width = MAX_WIDTH, bg=self.std_bg_color)
        self.body_box = tk.Frame(self.border_box, height = BODY_HEIGHT, width = MAX_WIDTH, bg=self.std_bg_color)

    def __layout(self):
        self.border_box.place(x=(int(HEIGHT)-MAX_HEIGHT)/2, y=(int(WIDTH)-MAX_WIDTH)/2)
        self.header_box.place(x=0, y=0)
        self.body_box.place(x=0, y=HEADER_HEIGHT + 1)
        self.window.title(WINDOW_TITLE)
        self.window.geometry(f"{WIDTH}x{HEIGHT}")

if __name__ == '__main__':
    window = tk.Tk()
    GUIController(window)
    window.mainloop()
