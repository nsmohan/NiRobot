#!/usr/bin/env python

"""
"  @file      rmct_sock_lib.py
"  @brief     Library to facilitate communication with RMCT proc
"  @details   Provides interfaces to communicate with RMCT
"  @author    Nitin Mohan
"  @date      April 7, 2020
"  @copyright 2020 - NM Technologies
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
import socket
import json

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#
from lib_py import getip
from lib_py.NMT_stdlib_py import NMT_result
from lib_py.proc_handle import ProcHandle

#---------------------------------------------------#
#                   Constants                       #
#---------------------------------------------------#
RMCT = "RMCT"

# -- Library Implementation -- #
class RMCTSockConnect(ProcHandle):

    def __init__(self, ip = ""):
        ProcHandle.__init__(self, RMCT, ip)

    #---------------------------------------------------#
    #                   Private Methods                 #
    #---------------------------------------------------#
    def __get_tx_message(self, motor, direction, angle, speed):

            """ 
            "  @brief              Compile Message for NiBot
            "  param[in] motor     Name of motor to be moved
            "  param[in] direction Direction to move in
            "  param[in] angle     Manually override motor angle
            "  param[in] speed     Set the speed of the motor
            """
            # -- Construct Message --#
            tx_message              = {}
            tx_message["type"]      = "hw_action"
            tx_message["motor"]     = motor
            tx_message["direction"] = direction
            tx_message["angle"]     = angle
            tx_message["speed"]     = speed

            return tx_message

    #---------------------------------------------------#
    #                   Public Methods                  #
    #---------------------------------------------------#
    def rx_message(self):

        """ 
        "  @brief             Get Message from RMCT
        "  @param[in] address Address of sender
        "  @param[in] message Message recieved
        "  @return            Message recieved from the server
        """

        return json.loads(self.proc.listen()[1])

    def tx_message(self, message):
        """ 
        "  @brief             Send message to proc
        "  @param[in] address message
        "  @return            None
        """

        print ("Sending Request .... {}".format(message))
        self.proc.tx_message(message)

    def construct_tx_message(self, actions):
        """ 
        "  @brief              Construct Array of TX Messages
        "  param[in] actions   Actions to be performed
        """

        return json.dumps(list(map(lambda action: self.__get_tx_message(action[0],
                                                                        action[1],
                                                                        action[2],
                                                                        action[3]), actions)))
