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
import json

#---------------------------------------------------#
#                   Constants                       #
#---------------------------------------------------#
RSXA_FILE = "/etc/NiBot/RSXA.json"

WIDTH  = "1600"
HEIGHT = "900"
MAX_WIDTH = 1580
MAX_HEIGHT = 880
HEADER_START = (int(HEIGHT) - MAX_HEIGHT)/2
HEADER_END   = HEADER_START + 200

MIN_X = 10
MIN_Y = 10
BODY_MIN_Y = 200
FOOTER_MIN_Y = 800
BODY_HEIGHT = FOOTER_MIN_Y - BODY_MIN_Y
MIN_GAP = 15
MIN_HEIGHT = 30
MIN_WIDTH = 100
BG_COLOR = "#F3F3F6"
MY_DIR = os.path.join(os.getcwd().split("NiRobot")[0], "NiRobot")
RUI_DIR = os.path.join(MY_DIR, "rui")
IMG_DIR = os.path.join(RUI_DIR, "imgs")

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#
class LayoutBase(object):

    def __init__(self):
        self.style = ttk.Style()
        self.__styles()

    def __styles(self):
        self.style.configure('std.TButton',
                             font = ('comic', 10, 'bold'), 
                             width=15) 
        self.style.configure('L.TButton',
                             font = ('comic', 20, 'bold'), 
                             width=15) 

    def new_button(self, window, text, style="std"):
        return ttk.Button(window, text=text, style=f"{style}.TButton")

class LayoutHeader(LayoutBase):
    def __init__(self, window):
        self.window = window
        self.__grpbox()
        self.__buttons()
        self.__images()
        self.__inputs()
        self.__layout()

    def __images(self):
        load = Image.open(os.path.join(IMG_DIR, "nibot.png"))
        render = ImageTk.PhotoImage(load)
        self.logo = tk.Label(self.window, image=render)
        self.logo.image = render

    def __inputs(self):
        self.host_txtbox = tk.Entry(self.connect_gb)

    def __buttons(self):
        self.cbtn =  self.new_button(self.connect_gb, "Connect")
        self.dcbtn = self.new_button(self.connect_gb, "Disconnect")

    def __grpbox(self):
        self.connect_gb = tk.LabelFrame(self.window, text="Connect-to-NiBot", bg=BG_COLOR)

    def __layout(self):
        self.logo.place(x=MIN_X, y=MIN_Y)
        self.connect_gb.place(x=1000, y=MIN_Y, height=70, width=500)
        self.host_txtbox.place(x=MIN_X, y=MIN_Y, height=MIN_HEIGHT, width=MIN_WIDTH)
        self.cbtn.place(x=MIN_WIDTH + MIN_GAP, y=MIN_Y)
        self.dcbtn.place(x=250, y=MIN_Y)

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
        super().__init__()
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

""" 
"  @class  SettingsTabView
"          Object to handle the RSXA Setings TreeView
"""
class SettingsTabView(LayoutBase):
    def __init__(self, window):

        """ 
        "  @brief Constructor for SettingsTabView
        "  @param[in] window -> Parent Element
        """

        self.window = window
        self.__tree()
        self.__fillTree()
        self.__buttons()
        self.__layout()

        self.toggle_simbtn["state"] = "disabled"
        self.edit_valuebtn["state"] = "disabled"

    def __buttons(self):
        self.toggle_simbtn = self.new_button(self.window, "TOGGLE SIM", "L")
        self.edit_valuebtn = self.new_button(self.window, "EDIT VALUE", "L")

    def __tree(self):

        """ 
        "  @brief Function to initialize the Tree Widget
        """

        self.settings_tree = ttk.Treeview(self.window, height=20)
        self.settings_tree["columns"] = ("item", "val")
        self.settings_tree.heading("#0",text="NiBot Setting",anchor=tk.W)
        self.settings_tree.heading("item",text="item",anchor=tk.W)
        self.settings_tree.heading("val",text="Value",anchor=tk.W)
        self.settings_tree.bind("<<TreeviewSelect>>", self.__handleSelect)

    def __fillTree(self):

        """ 
        "  @brief Read the RSXA settings and fill the tree
        """

        rsxa_settings = self.__read_rsxa_settings()
        rsxa_settings.pop("_comment")
        self.__parse_settings(rsxa_settings)


    def __layout(self):
        self.toggle_simbtn.place(x=MIN_X, y=450)
        self.edit_valuebtn.place(x=MIN_X * 32, y=450)

    def __read_rsxa_settings(self):

        """ 
        "  @brief Read the RSXA settings file
        """

        with open(RSXA_FILE, "r") as rsxa_file:
            return json.loads(rsxa_file.read())

    def __parse_settings(self, root, node="", index = "0", key_eq_item=False):
        """ 
        "  @brief Parse the RSXA settings and fill the tree
        """

        # -- Loop through all the items in the file -- #
        for key in root.keys():

            # -- Set iid value -- #
            iid = str(index) + key

            if type(root[key]) == list:

                # -- Insert the Parent Element -- #
                self.settings_tree.insert(node, 'end', iid, text=key)


                # -- Loop and fill the child elements -- #
                for i, item in enumerate(root[key]):

                    # -- Set sub id's -- #
                    new_index = index + str(i)
                    new_iid = iid + new_index

                    display_name, display_item, display_value, set_as_item, tag, = self.__set_list_items(key, item)
                    self.settings_tree.insert(iid, 'end', new_iid, text=display_name,
                                              value=(display_item, display_value), tags=tag)

                    # -- Repeat -- #
                    self.__parse_settings(item, node=new_iid, index=new_index, key_eq_item=set_as_item)
            else:

                # -- Refresh Variables for each iteration -- #
                val = ""
                item = ""
                val = str(root[key])
                display_name = key

                # -- Handle Special Requests -- #
                if key_eq_item:
                    display_name = ""
                    item = key

                # -- Insert Element into tree -- #
                self.settings_tree.insert(node, 'end', iid,  text=display_name, value=(item, val), tags=type(root[key]))

    def __set_list_items(self, key, item):

        """ 
        "  @brief Set elements for tree for list items
        "  @param[in] key -> Dictionary Key
        "  @param[in] item -> Dictionary items
        """

        # -- Initialize Variables -- #
        display_value = ""
        display_item = ""
        display_name = ""
        tag = ""
        set_as_item = False
        tags = None

        # -- Determine element type and take action -- #
        if key == "procs":
            display_name = item["proc_name"]
            item.pop("proc_name")
            set_as_item = True
        elif key == "hw":
            display_name = item["hw_name"]
            tag = "bool"
            display_value = str(item["hw_sim_mode"])
            display_item = "hw_sim_mode"
            item.pop("hw_name")
            item.pop("hw_sim_mode")
        elif key == "hw_interface":
            tag = "int"
            display_item = item["pin_name"]
            display_value = item["pin_no"]
            item.pop("pin_name")
            item.pop("pin_no")

        return display_name, display_item, display_value, set_as_item, tag

    def __handleSelect(self, event):

        """ 
        "  @brief Handle event if item is selected in tree
        "  @param[in] event
        """

        # -- Get Selected item -- #
        iid = self.settings_tree.selection()

        # -- Disable both buttons if morre than 1 item selected -- #
        if len(iid) > 1:
            self.edit_valuebtn["state"] = "disabled"
            self.toggle_simbtn["state"] = "disabled"
            return 
            
        item = self.settings_tree.item(iid)

        # -- Set Button States -- #
        if item["values"] and item["tags"]:
            if "bool" in item["tags"]:
                self.edit_valuebtn["state"] = "disabled"
                self.toggle_simbtn["state"] = "enabled"
            else:
                self.edit_valuebtn["state"] = "enabled"
                self.toggle_simbtn["state"] = "disabled"
        else:
            self.edit_valuebtn["state"] = "disabled"
            self.toggle_simbtn["state"] = "disabled"

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
        
class LayoutRoot(object):
    def __init__(self, window):
        self.window = window
        window.configure(bg=BG_COLOR)
        self.__Frames()
        self.header = LayoutHeader(self.border_box)
        self.body = LayoutBody(self.border_box)
        self.__layout()

    def __Frames(self):
        self.border_box = tk.Frame(self.window, height=MAX_HEIGHT, width=MAX_WIDTH, bg=BG_COLOR)

    def __layout(self):
        self.border_box.place(x=(int(HEIGHT)-MAX_HEIGHT)/2, y=(int(WIDTH)-MAX_WIDTH)/2)
        self.window.title("NiBot")
        self.window.geometry("%sx%s"%(WIDTH, HEIGHT))
    
class ConnectBox(LayoutRoot):
    def __grpbox(self):
        self.connect_gb = LabelFrame(self.window, text="Connect-to-Robot")

    def __layout(self):
        self.connect_gb.place(x=1200, y=MIN_Y, height=MIN_HEIGHT, width=310)

if __name__ == '__main__':
    window = tk.Tk()
    ConnectBox(window)
    window.mainloop()
