#!/usr/bin/env python

"""
"  @file      rsda_sock_lib.py
"  @brief     Library to handle communication with RSDA Process
"  @details   Interfaces to handle communication with RSDA Process
"  @author    Nitin Mohan
"  @date      December 14, 2020
"  @copyright 2020
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
import socket
import json

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#
from lib_py.proc_handle import ProcHandle
from lib_py.NMT_sock import NMT_sock

#---------------------------------------------------#
#                   Global Varibles                 #
#---------------------------------------------------#
PROC_NAME = "RSDA"

#--------------------------------------------------------------------------#
#                               Start of Program                           #
#--------------------------------------------------------------------------#
class RSDAProc(ProcHandle):

    def __init__(self, server_ip=""):

        """ 
        "  @brief              Constructor
        "  param[in] server_ip Host IP Address
        """

        ProcHandle.__init__(self, PROC_NAME, server_ip=server_ip)
        self.listener = NMT_sock("MULTICAST", "SERVER", self.client_ip, self.client_port)

    def start_listener(self, cbFunc, continous=True):

        """ 
        "  @brief           Start Listener for RSDA Socket
        "  param[in] cbFunc Function to be called when message is recieved
        """

        try:
            self.nibot.listen(cbFunc, continous=continous)
        except socket.timeout:
            print ("Socket timeoout occured. Stopping listener")

    def get_sensor_data(self):

        """ 
        "  @brief    Get and return the sensor Data      
        "  @return   sensor_data
        """

        return json.loads(self.listener.listen(False)[1])

if __name__ == '__main__':
    pass

