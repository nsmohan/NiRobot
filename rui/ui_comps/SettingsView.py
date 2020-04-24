#!/usr/bin/env python

"""
"  @file      SettingsView.py
"  @brief     Library to handle RSXA Settings Tree
"  @details   RSXA Treeview handler
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
from ui_comps.LayoutBase import LayoutBase

#---------------------------------------------------#
#                   Constants                       #
#---------------------------------------------------#
RSXA_FILE = "/etc/NiBot/RSXA.json"
MIN_X = 10

#---------------------------------------------------#
#                 Start of Program                  #
#---------------------------------------------------#

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

        # -- Initialize Class --#
        self.window = window
        self.__tree()
        self.__fillTree()
        self.__buttons()
        self.__layout()

        # -- Set Default States -- #
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
        self.toggle_simbtn.update()
        print (self.toggle_simbtn.winfo_width())

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

# --- End of File ----- #
