#!/usr/bin/env python

"""
"  @file      proc_handler.py
"  @brief     Parent Class for Handling NiBot Procs
"  @details   This library contains interfaces to allow the user to communicate with
              Nibot proccess. 
"  @author    Nitin Mohan 
"  @date      December 9, 2020
"  @copyright 2020
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
import os
import json

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#
from lib_py.NMT_sock import NMT_sock

#---------------------------------------------------#
#                   Global Varibles                 #
#---------------------------------------------------#
""" @var RSXA_FILE Path to RSXA.json file """
RSXA_FILE = os.environ["RSXA_SETTINGS"]


#--------------------------------------------------------------------------#
#                               Start of Program                           #
#--------------------------------------------------------------------------#

class ProcHandle(object):

    def __init__(self, proc_name, server_ip="", client_ip=""):
        self.server_ip = server_ip
        self.client_ip = client_ip
        self.proc_name = proc_name
        self.__rsxa_settings()
        self.proc = self.__creat_tcp_connection()

    def __rsxa_settings(self):

        """ 
        "  @brief  Read the Robot Settings file and get needed settings
        """

        # -- Read the File -- #
        with open(RSXA_FILE, "r") as rsxa_file:
            rsxa = json.load(rsxa_file)

        # -- Find the Settings Needed -- #
        proc = list(filter(lambda p: p["proc_name"] == self.proc_name, rsxa["procs"]))[0]
        
        # -- Load the Settings into the object -- #
        if not self.server_ip: self.server_ip   = proc["server_ip"]
        if not self.client_ip: self.client_ip   = proc["client_ip"]
        self.server_port                        = proc["server_p"]
        self.client_port                        = proc["client_p"]

    def __creat_tcp_connection(self):
        """ 
        "  @brief  Create TCP Connection with the Proc
        """

        return NMT_sock("TCP", "CLIENT", self.server_ip, self.server_port)

    #---------------------------------------------------#
    #                   Public Methods                  #
    #---------------------------------------------------#
    
    def show_rsxa_settings(self):

        """ 
        "  @brief Show RSXA Settings on the screen
        """
        print ("server_ip=%s\nclient_ip=%s\nserver_port=%d\nclient_port=%d"%(self.server_ip, 
                                                                             self.client_ip,
                                                                             self.server_port,
                                                                             self.client_port))
        
    def stop_proc(self):

        """ 
        "  @brief Send Message to Stop Proc
        """

        print("Stopping {} Task......".format(self.proc_name))
        message = json.dumps([({"type": "proc_action", "action": "exit"})])
        self.proc.tx_message(message)

if __name__ == '__main__':
    pass

