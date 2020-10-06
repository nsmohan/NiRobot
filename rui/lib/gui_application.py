#!/usr/bin/env python

"""
"  @file      gui_application.py
"  @brief     GUI Middleware
"  @details   Application layer to which provides interfaces to 
              communicate with the nibot
"  @author    Nitin Mohan
"  @date      May 07, 2020
"  @copyright 2020 - NM Technologies
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
import zope.event
import json
import time

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#
from lib_py.NMT_transport import NMT_transport
from lib_py.NMT_stdlib_py import NMT_result
from lib_py.rmct_sock_lib import RMCTSockConnect

#---------------------------------------------------#
#                   Constants                       #
#---------------------------------------------------#
LOCAL_RSXA_FILE  = "/etc/NiBot/RSXA.json"
REMOTE_RSXA_FILE = "/etc/NiBot/RSXA.json"
RMCT_TASK        = "/home/nibot/NiRobot/bld/RMCT"
RUIxSETTINGS     = "/etc/NiBot/RUIxSettings.json"

""" 
"  @class  GUI_Application
"          Object for interacting with NiBot
"""

class GUI_Application(object):

    def __init__(self):

        """ 
        "  @brief Class Init
        """

        self.rui_settings = self.__read_rui_settings()
        zope.event.subscribers.append(self.__handle_exit)


    def __read_rsxa_settings(self):

        """ 
        "  @brief Read the RSXA settings file
        """

        with open(LOCAL_RSXA_FILE, "r") as rsxa_file:
            return json.loads(rsxa_file.read())

    def __read_rui_settings(self):

        """ 
        "  @brief Read the RUI Settings File
        """

        with open(RUIxSETTINGS, "r") as rui_settings:
            return json.loads(rui_settings.read())

    def __write_rui_settings(self):

        """ 
        "  @brief Write RUI Settings to File
        """

        with open(RUIxSETTINGS, "w") as rui_settings_file:
            json.dump(self.rui_settings, rui_settings_file, indent=4)

    def __write_rsxa_settings(self, rsxa_settings):

        """ 
        "  @brief Write RSXA Settings to File
        """

        with open(LOCAL_RSXA_FILE, "w") as rsxa_settings_file:
            json.dump(rsxa_settings, rsxa_settings_file, indent=4)

    def __handle_exit(self, event):

        """ 
        "  @brief Handle Exit Event
        """

        if event == "exit":

            # -- 1. Write RUI Settings --#
            self.__write_rui_settings()

    def connect_to_nibot(self, ip_address):

        """ 
        "  @brief Connect to NiBot and create instnace of object
        """

        #-- Store IP Address in Object --#
        self.ip_address = ip_address

        try:
            self.nibot = NMT_transport(ip_address,
                                       username="nibot",
                                       password="nibot")
        except Exception as e:
            print(e)
            raise Exception ("Unable to Connect to NiBot!")
        else:

            #-- Update Hosts in RUI Settings --#
            if self.ip_address not in self.rui_settings['known_host']:
                self.rui_settings['known_host'].append(self.ip_address)

            #-- Run RMCT --#
            if not GUI_Application.is_task_running(self.nibot, "RMCT"):
                print("RMCT Task not running in NiBot...... ")
                GUI_Application.start_task(self.nibot, RMCT_TASK)
                time.sleep(1) #2 Second Delay to allow the process to start

            #-- RMCT Socket --#
            self.connect_to_rmct()

            #-- Raise Connected Event --#
            zope.event.notify("connected")
            print("SSH Connection Successful!")

    def disconnect_from_nibot(self):

        """ 
        "  @brief Disconnect from NiBot and delete object
        """

        del(self.nibot)
        zope.event.notify("disconnected")

    def update_rsxa_settings_on_nibot(self, rsxa_settings):

        """ 
        "  @brief Send RSXA Settings file to NiBot
        "  @param[in] rsxa_settings
        """

        # -- Send the Settings File --#
        try:
            self.send_rsxa_settings_to_nibot(rsxa_settings)
        except Exception as e:
            print(e)
            raise Exception("Failed to Update RSXA Settings")
        else:
            if GUI_Application.is_task_running(self.nibot, "RMCT"):

                print("Restarting RMCT Task.....")
                self.rmct.stop_rmct()

                time.sleep(1)  #Delay to allow task to exit

                GUI_Application.start_task(self.nibot, RMCT_TASK)
                time.sleep(1) #1 Second Delay to allow the process to start

                #-- RMCT Socket --#
                self.connect_to_rmct()

    def send_rsxa_settings_to_nibot(self, rsxa_settings):

        """ 
        "  @brief Send RSXA Settings file to NiBot
        "  @param[in] rsxa_settings
        """

        try:
            self.__write_rsxa_settings(rsxa_settings)
            self.nibot.send_file(LOCAL_RSXA_FILE, REMOTE_RSXA_FILE)
        except Exception as e:
            raise Exception("Failed to send RSXA.json file")

    def get_rsxa_settings_from_nibot(self):

        """ 
        "  @brief Send RSXA Settings file to NiBot
        """

        try:
            self.nibot.get_file(REMOTE_RSXA_FILE, LOCAL_RSXA_FILE)
            return self.__read_rsxa_settings()
        except Exception as e:
            print(e)
            raise Exception("Failed to Get RSXA JSON File!")


    def perform_nibot_motor_action(self, actions):

        """ 
        "  @brief Perform NiBot Motor Action
        "  @param[in] motor
        "  @param[in] direction
        "  @param[in] angle
        "  @param[in] speed
        """

        # -- Construct TX Message --#
        tx_message = self.rmct.construct_tx_message(actions)

        # --Send the Message --#
        self.rmct.tx_message(tx_message)

        #-- Wait for the Response -- #
        response = self.rmct.rx_message()

        if response:
            print("NiBOT Response=%s"%(NMT_result.get_result(response["result"])))
        else:
            raise Exception("NiBot Motor Action Failure!")

    @staticmethod
    def is_task_running(ssh_server, task):

        """ 
        "  @brief Method to check if a task is running
        "  @param[in] task - task to look for
        "  @param[out] task_running - (True = running | False = Not running)
        """

        #-- Check if the task is running --#
        stdout, stderr = ssh_server.send_command(f"pgrep {task}")

        # -- Exit the function --#
        return stdout.decode("utf-8") != ""

    @staticmethod
    def start_task(ssh_server, task):

        """ 
        "  @brief Start a task over SSH
        "  @param[in] task - task to start
        """

        # -- Starting the task -- #
        print ("Starting task ...", task) 
        ssh_server.send_command(task, blocking=False)

    def connect_to_rmct(self):

        """ 
        "  @brief Start a task over SSH
        """

        # -- Connect to RMCT Socket --#
        print("Creating Socket Connection with RMCT")
        self.rmct = RMCTSockConnect(self.ip_address)

    def get_known_hosts(self):

        """ 
        "  @brief Get list of known hosts
        """

