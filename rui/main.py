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


#---------------------------------------------------#
#                   Constants                       #
#---------------------------------------------------#
WINDOW_TITLE = "NiBot"


MIN_X = 10
MIN_Y = 10
BODY_MIN_Y = 200
FOOTER_MIN_Y = 800
BODY_HEIGHT = FOOTER_MIN_Y - BODY_MIN_Y
MIN_GAP = 15
MIN_HEIGHT = 30
MIN_WIDTH = 100
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

class CameraControlBox(LayoutBase):
    def __init__(self, window):
        self.window = window
        self.__grpbox()
        self.__inputs()
        self.__labels()
        self.__buttons()
        self.__layout()

    def __inputs(self):
        self.angle_txbx = tk.Entry(self.camctrl_gb)

    def __labels(self):
        self.angle_label = tk.Label(self.camctrl_gb, text="Enter Angle:")

    def __grpbox(self):
        self.camctrl_gb = ttk.LabelFrame(self.window, text="Camera Control", width=350, height=150)

    def __buttons(self):
        self.upbtn    = self.new_button(self.camctrl_gb, "UP")
        self.downbtn  = self.new_button(self.camctrl_gb, "DOWN")
        self.leftbtn  = self.new_button(self.camctrl_gb, "LEFT")
        self.rightbtn = self.new_button(self.camctrl_gb, "RIGHT")
        self.custbtn  = self.new_button(self.camctrl_gb, "CUSTOM")

    def __layout(self):
        self.upbtn.place(x=100, y=0)
        self.downbtn.place(x=100, y=60)
        self.leftbtn.place(x=30, y=30)
        self.rightbtn.place(x=170, y=30)
        self.angle_label.place(x=0, y=90, height=MIN_HEIGHT, width=MIN_WIDTH)
        self.angle_txbx.place(x=100, y=90, height=MIN_HEIGHT, width=MIN_WIDTH)
        self.custbtn.place(x=200, y=90)

class DriveControlBox(LayoutBase):
    def __init__(self, window):
        self.window = window
        self.direction = tk.StringVar()
        self.__grpbox()
        self.__inputs()
        self.__radiobuttons()
        self.__labels()
        self.__buttons()
        self.__layout()

    def __inputs(self):
        self.speed_txbx = tk.Entry(self.drive_ctrl_gb)

    def __radiobuttons(self):
        self.fwdrad = tk.Radiobutton(self.drive_ctrl_gb, text="Forward", variable=self.direction, value="FORWARD")
        self.revrad = tk.Radiobutton(self.drive_ctrl_gb, text="Reverse", variable=self.direction, value="REVERSE")
        self.stoprad = tk.Radiobutton(self.drive_ctrl_gb, text="Stop",   variable=self.direction, value="STOP")

    def __labels(self):
        self.speed_label = tk.Label(self.drive_ctrl_gb, text="Speed:")

    def __grpbox(self):
        self.drive_ctrl_gb = ttk.LabelFrame(self.window, text="Drive Control", width=350, height = 150)

    def __buttons(self):
        self.left_mtr_btn    = self.new_button(self.drive_ctrl_gb, "LEFT MOTOR")
        self.right_mtr_btn  = self.new_button(self.drive_ctrl_gb, text="RIGHT MOTOR")

    def __layout(self):
        self.left_mtr_btn.place(x=120, y=MIN_Y)
        self.right_mtr_btn.place(x=120, y=MIN_Y + 40)
        self.fwdrad.place(x=MIN_X, y=MIN_Y)
        self.revrad.place(x=MIN_X, y=MIN_Y + 20)
        self.stoprad.place(x=MIN_X, y=MIN_Y + 40)
        self.speed_label.place(x=MIN_X, y=MIN_Y + 85)
        self.speed_txbx.place(x=70, y=MIN_Y + 80, height=MIN_HEIGHT, width=MIN_WIDTH)
        self.drive_ctrl_gb.place(x=MIN_X, y=MIN_Y + 150)


class LayoutBody(object):
    def __init__(self, window):
        self.window = window
        self.__tabs()
        self.cam_ctrl = CameraControlBox(self.control_tab)
        self.drive_ctrl = DriveControlBox(self.control_tab)
        self.settings_view = SettingsTabView(self.settings_tab)
        self.__layout()


    def __tabs(self):
        self.bodytab = ttk.Notebook(self.window)
        self.settings_tab = ttk.Frame(self.bodytab)
        self.control_tab = ttk.Frame(self.bodytab)
        self.bodytab.add(self.control_tab, text="Control")
        self.bodytab.add(self.settings_tab, text="Settings")

    def __layout(self):
        self.bodytab.place(x=MIN_X, y=BODY_MIN_Y, height=BODY_HEIGHT, width=int(WIDTH) - 20)
        self.cam_ctrl.camctrl_gb.place(x=MIN_X, y=MIN_Y)
        self.drive_ctrl.drive_ctrl_gb.place(x=MIN_X, y=MIN_Y + 150)
        self.settings_view.settings_tree.place(x=MIN_X, y=MIN_Y)
        
class GUIController(LayoutBase):
    def __init__(self, window):
        super().__init__()
        self.window = window
        window.configure(bg=self.std_bg_color)
        self.__Frames()
        self.__layout()
        self.header = LayoutHeader(self.header_box)
        self.body = LayoutBody(self.border_box)

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
