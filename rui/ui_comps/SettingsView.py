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
from tkinter import simpledialog
import copy
import zope.event

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#
from ui_comps.LayoutBase import *

#---------------------------------------------------#
#                   Constants                       #
#---------------------------------------------------#
SETTINGS_TREE_HEIGHT = BODY_HEIGHT - 200

#---------------------------------------------------#
#                 Start of Program                  #
#---------------------------------------------------#

""" 
"  @class  SettingsTabView
"          Object to handle the RSXA Setings TreeView
"""
class SettingsTabView(LayoutBase):

    def _class_comps_init(self):

        # -- Configure Call Backs-- #
        zope.event.subscribers.append(self.__update_btn_states_cb)
        zope.event.subscribers.append(self.__refresh_rsxa_settings_cb)

        # -- Send Init Event -- #
        zope.event.notify("init")

    def _buttons(self):

        """ 
        "  @brief SettingsView Buttons
        """

        self.toggle_simbtn = self.new_button(self.window, "TOGGLE SIM", "L", command=self.__handle_toggle_sim)
        self.edit_valuebtn = self.new_button(self.window, "EDIT VALUE", "L", command=self.__handle_change_setting)
        self.undo_changesbtn = self.new_button(self.window, "UNDO CHANGES", "L", command=self.__handle_undo_changes)
        self.apply_btn = self.new_button(self.window, "APPLY CHANGES", "L", command=self.__handle_apply_changes)

    def _tree(self):

        """ 
        "  @brief Function to initialize the Tree Widget
        """

        self.settings_tree = ttk.Treeview(self.window)
        self.settings_tree["columns"] = ("item", "val")
        self.settings_tree.heading("#0",text="NiBot Setting",anchor=tk.W)
        self.settings_tree.heading("item",text="item",anchor=tk.W)
        self.settings_tree.heading("val",text="Value",anchor=tk.W)
        self.settings_tree.bind("<<TreeviewSelect>>", self.__handleSelect)

    def _layout(self):

        """ 
        "  @brief Settings View Layout
        """

        self.settings_tree.place(x=0, y=0, height=SETTINGS_TREE_HEIGHT)
        self.toggle_simbtn.place(x=0, y=SETTINGS_TREE_HEIGHT + MIN_X)
        self.edit_valuebtn.place(x=self.L_button_width, y=SETTINGS_TREE_HEIGHT + MIN_X)
        self.undo_changesbtn.place(x=0, y=SETTINGS_TREE_HEIGHT + MIN_X + self.L_button_height)
        self.apply_btn.place(x=self.L_button_width, y = SETTINGS_TREE_HEIGHT + MIN_X + self.L_button_height)
        
    def __fillTree(self, rsxa_settings):

        """ 
        "  @brief Fille the Tree
        "  @param[in] rsxa_settings
        """

        rsxa_settings.pop("_comment")
        self.__parse_settings(rsxa_settings)



    def __parse_settings(self, root, node="", index = "0", key_eq_item=False):
        """ 
        "  @brief Parse the RSXA settings and fill the tree
        "  @param[in] root
        "  @param[in] node
        "  @param[in] index
        "  @param[in] key_eq_item
        """

        # -- Loop through all the items in the file -- #
        for key in root.keys():

            # -- Set iid value -- #
            iid = f"{index}-{key}"

            if type(root[key]) == list:

                # -- Insert the Parent Element -- #
                self.settings_tree.insert(node, 'end', iid, text=key)

                # -- Loop and fill the child elements -- #
                for i, item in enumerate(root[key]):

                    # -- Set sub id's -- #
                    new_index = index + str(i)
                    new_iid = f"{iid}-{new_index}"

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

    # --------------------------------------------------------------#
    #                  Handle User Inputs                           #
    #---------------------------------------------------------------#
    def __handleSelect(self, event):

        """ 
        "  @brief Handle event if item is selected in tree
        "  @param[in] event
        """

        # -- Get Selected item -- #
        iid = self.settings_tree.selection()

        # -- Disable both buttons if morre than 1 item selected -- #
        if len(iid) > 1:
            zope.event.notify("na_select")
            return 
            
        item = self.settings_tree.item(iid)

        # -- Set Button States -- #
        if item["values"] and item["tags"]:
            if "bool" in item["tags"]:
                zope.event.notify("bool_select")
            else:
                zope.event.notify("other_select")
        else:
            zope.event.notify("na_select")

    def __handle_toggle_sim(self):

        """ 
        "  @brief Handle Toggle Sim Action
        """

        #-- Get the Selected item --#
        iid, items, text = self.__get_selected_item()

        # --Set New Sim Value --#
        new_sim = not(items[1] == "True")
        self.__update_tree(iid, (items[0], str(new_sim)))

        # -- Update the value in Memory -- #
        hw_item = list(filter(lambda hw: hw["hw_name"] == text, self.rsxa_settings_mem["hw"]))[0]
        hw_item["hw_sim_mode"] = new_sim

    def __handle_change_setting(self):

        """ 
        "  @brief Edit Value Handle
        """

        # -- Initialize Variables -- #
        new_setting = None

        #-- Get the Selected item --#
        iid, items, text = self.__get_selected_item()

        #-- Check Item name --#
        if items[0] == "":
            display_text = text
        else:
            display_text = items[0]

        #-- Get User Input --#
        new_setting = self.__showdialog(type(items[1]), display_text, items[1])

        #-- Update Tree Value --#
        if new_setting != None and new_setting != items[1]:
            root_key = self.__get_treeroot(iid)

            if root_key == "hw":
                indexes = iid[0][-3:]
                index1 = int(indexes[-2])
                index2 = int(indexes[-1])
                hw_interface = self.rsxa_settings_mem["hw"][index1]["hw_interface"][index2]
                hw_interface["pin_no"] = new_setting
            elif root_key == "procs":
                index = int(iid[0][1])
                key = iid[0].split("-")[1]
                proc = self.rsxa_settings_mem["procs"][index]
                proc[key] = new_setting
                
            self.__update_tree(iid, (items[0], new_setting))

    def __handle_undo_changes(self):

        """ 
        "  @brief Implementation of Undo Changes Button
        """

        self.settings_tree.delete(*self.settings_tree.get_children())
        self.__fillTree(copy.deepcopy(self.rsxa_settings))
        zope.event.notify("reset")


    def __handle_apply_changes(self):

        """ 
        "  @brief Implementation of Undo Changes Button
        """

        try:
            self.nibot_ap.send_rsxa_settings_to_nibot(self.rsxa_settings_mem)
            zope.event.notify("reset")
        except Exception as e:
            self.throw_error(f"Update Settings Failed! {e}")

    # --------------------------------------------------------------#
    #                  Facility Functions                           #
    #---------------------------------------------------------------#
    def __showdialog(self, dialog_type, item_name, item_value):

        """ 
        "  @brief Show Dialog to get user Entry
        "  param[in] dialog_type
        "  param[in] item_name
        "  param[in] item_value
        """

        title = "Edit NiBot Setting"

        if dialog_type == str:
            return simpledialog.askstring(title, 
                                          f"Enter New {item_name}:",
                                          initialvalue=item_value,
                                          parent=self.window)
        elif dialog_type == int:
            return simpledialog.askinteger(title, 
                                          f"Enter New {item_name}:",
                                          parent=self.window,
                                          initialvalue=item_value,
                                          minvalue = 0,
                                          maxvalue = 6000)

    def __get_selected_item(self):

        """ 
        "  @brief Get the selected item
        """

        #-- Get the Tree Item --#
        iid = self.settings_tree.selection()
        items = self.settings_tree.item(iid)

        #-- End of Function --#
        return iid, items["values"], items["text"]

    def __update_tree(self, iid, items):

        """ 
        "  @brief Update Tree Values
        "  @param[in] iid
        "  @param[in] items
        """
        
        self.settings_tree.item(iid, value=(items[0], items[1]))
        zope.event.notify("update")

    def __get_treeroot(self, iid):

        """ 
        "  @brief Return the root element
        "  param[in] iid
        """

        # -- Get the Root Key -- #
        while iid != "":
            piid = iid
            iid = self.settings_tree.parent(iid)

        #-- Exit the Function --#
        return piid.split("-")[1]


    # -----------------------------------------------------------#
    #                  Call Back Functions                       #
    #------------------------------------------------------------#
    def __update_btn_states_cb(self, event):

        """ 
        "  @brief Tree Updated Event Handler
        "  param[in] event
        """

        if event == "init":
            self.toggle_simbtn["state"] = "disabled"
            self.edit_valuebtn["state"] = "disabled"
            self.undo_changesbtn["state"] = "disabled"
            self.apply_btn["state"] = "disabled"
        elif event == "reset":
            self.undo_changesbtn["state"] = "disabled"
            self.apply_btn["state"] = "disabled"
        elif event == "update":
            self.undo_changesbtn["state"] = "enabled"
            self.apply_btn["state"] = "enabled"
        elif event == "bool_select":
            self.toggle_simbtn["state"] = "enabled"
            self.edit_valuebtn["state"] = "disabled"
        elif event == "other_select":
            self.toggle_simbtn["state"] = "disabled"
            self.edit_valuebtn["state"] = "enabled"
        elif event == "na_select":
            self.toggle_simbtn["state"] = "disabled"
            self.edit_valuebtn["state"] = "disabled"
            
    def __refresh_rsxa_settings_cb(self, event):

        """ 
        "  @brief Update the Tree with Latest RSXA Settings
        "  param[in] event
        """

        if event == "connected" or event == "reset":
            try:


                # -- Delete any items -- #
                self.settings_tree.delete(*self.settings_tree.get_children())

                #-- Get Settings from NiBot --#
                self.rsxa_settings = self.nibot_ap.get_rsxa_settings_from_nibot()
                
                # -- Copy Settings into Memory --#
                self.rsxa_settings_mem = copy.deepcopy(self.rsxa_settings)

                # -- Fill the Tree -- #
                self.__fillTree(copy.deepcopy(self.rsxa_settings))

            except Exception as e: 
                self.throw_error(f"Failed to Update RSXA Settings! {e}")

# --- End of File ----- #
