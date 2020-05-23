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


    def __wrtie_rsxa_settings(self, rsxa_settings):

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
            self.nibot = NMT_transport(ip_address)
            zope.event.notify("connected")
        except Exception as e:
            print(e)
            raise Exception ("Unable to Connect to NiBot!")

        print("SSH Connection Successful!")
        self.rmct = RMCTSockConnect()

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
            self.__wrtie_rsxa_settings(rsxa_settings)
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
        tx_message = \
            list(map(lambda action: self.rmct.construct_tx_message(action), actions))

        # --Send the Message --#
        self.rmct.tx_message(tx_message)

        #-- Wait for the Response -- #
        response = self.rmct.rx_message()

        if response:
            print("NiBOT Response=%s"%(NMT_result.get_result(response["result"])))
        else:
            raise Exception("NiBot Motor Action Failure!")

