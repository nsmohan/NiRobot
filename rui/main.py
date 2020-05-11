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
import zope.event

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#
from ui_comps.SettingsView import SettingsTabView
from ui_comps.LayoutBase import *
from ui_comps.ConnectBox import ConnectBox 
from ui_comps.control_tab import CameraControlBox 
from ui_comps.control_tab import DriveMotorControlBox 
from lib.gui_application import GUI_Application 

#---------------------------------------------------#
#                   Constants                       #
#---------------------------------------------------#
WINDOW_TITLE = "NiBot"
MY_DIR = os.path.join(os.getcwd().split("NiRobot")[0], "NiRobot")
RUI_DIR = os.path.join(MY_DIR, "rui")
IMG_DIR = os.path.join(RUI_DIR, "imgs")


class LayoutHeader(LayoutBase):

    def _class_comps_init(self):
        ConnectBox(self.window, nibot_ap)

    def _images(self):
        load = Image.open(os.path.join(IMG_DIR, "nibot.png"))
        render = ImageTk.PhotoImage(load)
        self.logo = tk.Label(self.window, image=render)
        self.logo.image = render

    def _layout(self):
        self.logo.place(x=0, y=0)


class LayoutBody(LayoutBase):

    def _class_comps_init(self):
        self.cam_ctrl = CameraControlBox(self.control_tab, self.nibot_ap)
        self.drive_ctrl = DriveMotorControlBox(self.control_tab, self.nibot_ap)
        self.settings_view = SettingsTabView(self.rsxa_settings_box, self.nibot_ap)

    def _class_vars_init(self):
        self.settings_tab_width = 700

    def _frames(self):
        self.rsxa_settings_box = tk.Frame(self.settings_tab, height=BODY_HEIGHT, width=self.settings_tab_width)

    def _tabs(self):
        self.bodytab = ttk.Notebook(self.window)
        self.settings_tab = ttk.Frame(self.bodytab)
        self.control_tab = ttk.Frame(self.bodytab)
        self.bodytab.add(self.control_tab, text="Control")
        self.bodytab.add(self.settings_tab, text="Settings")

    def _layout(self):
        self.bodytab.place(x=0, y=0, height=BODY_HEIGHT, width=MAX_WIDTH - 10)
        self.cam_ctrl.camctrl_gb.place(x=MIN_X, y=MIN_Y)
        self.rsxa_settings_box.place(x=MIN_X, y=MIN_Y)
        self.drive_ctrl.drive_ctrl_gb.place(x=MIN_X, y=MIN_Y + 200)

    def disable_body_tabs(self):
        self.bodytab.tab(0, state="disabled")
        self.bodytab.tab(1, state="disabled")

    def enable_body_tabs(self):
        self.bodytab.tab(0, state="normal")
        self.bodytab.tab(1, state="normal")
        self.bodytab.select(0)
        
class GUIController(LayoutBase):

    def _class_comps_init(self):
        self.window.configure(bg=self.std_bg_color)
        self.header = LayoutHeader(self.header_box, self.nibot_ap)
        self.body = LayoutBody(self.body_box, self.nibot_ap)
        zope.event.subscribers.append(self.__handle_button_states)
        self.body.disable_body_tabs()

    def _frames(self):
        self.border_box = tk.Frame(self.window, height=MAX_HEIGHT, width=MAX_WIDTH, bg=self.std_bg_color)
        self.header_box = tk.Frame(self.border_box, height = HEADER_HEIGHT, width = MAX_WIDTH, bg=self.std_bg_color)
        self.body_box = tk.Frame(self.border_box, height = BODY_HEIGHT, width = MAX_WIDTH, bg=self.std_bg_color)

    def _layout(self):
        self.border_box.place(x=(int(HEIGHT)-MAX_HEIGHT)/2, y=(int(WIDTH)-MAX_WIDTH)/2)
        self.header_box.place(x=0, y=0)
        self.body_box.place(x=0, y=HEADER_HEIGHT + 1)
        self.window.title(WINDOW_TITLE)
        self.window.geometry(f"{WIDTH}x{HEIGHT}")

    def __handle_button_states(self, event):

        if event == "connected":
            self.body.enable_body_tabs()
        elif event == "disconnected":
            self.body.disable_body_tabs()





if __name__ == '__main__':
    window = tk.Tk()
    nibot_ap = GUI_Application()
    GUIController(window, nibot_ap)
    window.mainloop()
