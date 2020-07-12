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
import struct
import json

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#
from lib_py import getip
from lib_py.NMT_stdlib_py import NMT_result

#---------------------------------------------------#
#                   Constants                       #
#---------------------------------------------------#
""" @var RSXA_FILE Path to RSXA.json file """
RSXA_FILE = "/etc/NiBot/RSXA.json"

RMCT = "RMCT"
MAX_BUFF_SIZE = 4096

# -- Library Implementation -- #
class RMCTSockConnect(object):

    def __init__(self, ip_address=""):
        self.__rsxa_settings(ip_address)
        self.__create_and_connect_socket()
    
    #---------------------------------------------------#
    #                   Private Methods                 #
    #---------------------------------------------------#
    def __del__(self):
        self.client_sock.close()

    def __create_and_connect_socket(self):

        """ 
        "  @brief  Construct TCP Socket and Connect to RMCT
        """

        self.client_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.client_sock.connect((self.rmct_server_ip, self.rmct_server_port))
        
    def __rsxa_settings(self, ip_address):

        """ 
        "  @brief  Read the Robot Settings file and get needed settings
        """

        # -- Read the File -- #
        with open(RSXA_FILE, "r") as rsxa_file:
            rsxa = json.load(rsxa_file)

        # -- Find the Settings Needed -- #
        rmct_proc = list(filter(lambda p: p["proc_name"] == RMCT, rsxa["procs"]))[0]
        
        # -- Load the Settings into the object -- #
        if ip_address:
            # -- Override IP Address --#
            self.rmct_server_ip = ip_address
        else:
            self.rmct_server_ip = rmct_proc["server_ip"]
        self.rmct_server_port = rmct_proc["server_p"]

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
    
    def show_rsxa_settings(self):

        """ 
        "  @brief Show RSXA Settings on the screen
        """

        print ("server_ip=%s\nclient_ip=%s\nserver_port=%d\nclient_port=%d"%(self.rmct_server_ip, 
                                                                             self.rmct_client_ip,
                                                                             self.rmct_server_port,
                                                                             self.rmct_client_port))

    def tx_message(self, message):
        """ 
        "  @brief                Send Message to RMCT over the socket
        "  @param[in]            message -> Message to send
        """

        print ("Sending request to NiBot ..... {}".format(message))
        self.client_sock.send(message.encode())

    def rx_message(self):
        """ 
        "  @brief          Recieve message from RMCT
        "  @return         Message recieved from the server
        """

        try:
            return json.loads(self.client_sock.recv(MAX_BUFF_SIZE))
        except socket.timeout:
            return False

    def construct_tx_message(self, actions):
        """ 
        "  @brief              Construct Array of TX Messages
        "  param[in] actions   Actions to be performed
        """

        return json.dumps(list(map(lambda action: self.__get_tx_message(action[0],
                                                                        action[1],
                                                                        action[2],
                                                                        action[3]), actions)))
