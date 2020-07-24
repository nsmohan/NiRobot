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

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#
from lib_py.NMT_transport import NMT_transport
from lib_py.NMT_stdlib_py import NMT_result
from lib_py.rmct_sock_lib import RMCTSockConnect

#---------------------------------------------------#
#                   Constants                       #
#---------------------------------------------------#
LOCAL_RSXA_FILE = "/etc/NiBot/RSXA.json"
REMOTE_RSXA_FILE = "/etc/NiBot/RSXA.json"
RMCT_TASK = "/home/nibot/NiRobot/tools/test.sh"

""" 
"  @class  GUI_Application
"          Object for interacting with NiBot
"""

class GUI_Application(object):

    def __read_rsxa_settings(self):

        """ 
        "  @brief Read the RSXA settings file
        """

        with open(LOCAL_RSXA_FILE, "r") as rsxa_file:
            return json.loads(rsxa_file.read())


    def __write_rsxa_settings(self, rsxa_settings):

        """ 
        "  @brief Write RSXA Settings to File
        """

        with open(LOCAL_RSXA_FILE, "w") as rsxa_settings_file:
            json.dump(rsxa_settings, rsxa_settings_file, indent=4)

    def connect_to_nibot(self, ip_address):

        """ 
        "  @brief Connect to NiBot and create instnace of object
        """

        try:
            self.nibot = NMT_transport(ip_address,
                                       username="nibot",
                                       password="nibot")
        except Exception as e:
            print(e)
            raise Exception ("Unable to Connect to NiBot!")
        else:
            if not GUI_Application.is_task_running(self.nibot, "RMCT"):
                print("RMCT Task not running in NiBot...... ")
                GUI_Application.start_task(self.nibot, RMCT_TASK)
                print("Stared RMCT....")

            # -- Connect to RMCT Socket --#
            print("Creating Socket Connection with RMCT")
            self.rmct = RMCTSockConnect(ip_address)
                
            #-- Raise Connected Event --#
            zope.event.notify("connected")
            print("SSH Connection Successful!")

    def disconnect_from_nibot(self):

        """ 
        "  @brief Disconnect from NiBot and delete object
        """

        del(self.nibot)
        zope.event.notify("disconnected")

    def send_rsxa_settings_to_nibot(self, rsxa_settings):

        """ 
        "  @brief Send RSXA Settings file to NiBot
        """

        try:
            self.__write_rsxa_settings(rsxa_settings)
            self.nibot.send_file(LOCAL_RSXA_FILE, REMOTE_RSXA_FILE)
        except Exception as e:
            print(e)
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
        print (stdout.decode("utf-8"))
        return stdout.decode("utf-8") != ""

    @staticmethod
    def start_task(ssh_server, task):

        """ 
        "  @brief Start a task over SSH
        "  @param[in] task - task to start
        """

        # -- Starting the task -- #
        print ("Starting task ...", task) 
        stdout, stderr = ssh_server.send_command(task, blocking=False)

