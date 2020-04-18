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
SOCK_TIMEOUT  = 2

# -- Library Implementation -- #
class RMCTSockConnect(object):

    def __init__(self):
        self.ip = str(getip.get_local_ip())
        self.__rsxa_settings()
        self.multi_sock_tx = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
        self.multi_sock_rx = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
        self.multi_sock_rx.settimeout(SOCK_TIMEOUT)
        self.__config_multicast()
    
    def __del__(self):
        self.multi_sock_tx.close()
        self.multi_sock_rx.close()
    
    def __rsxa_settings(self):

        """ 
        "  @brief  Read the Robot Settings file and get needed settings
        """

        # -- Read the File -- #
        with open(RSXA_FILE, "r") as rsxa_file:
            rsxa = json.load(rsxa_file)

        # -- Find the Settings Needed -- #
        rmct_proc = filter(lambda p: p["proc_name"] == RMCT, rsxa["procs"])[0]

        # -- Load the Settings into the object -- #
        self.rmct_server_ip = rmct_proc["server_ip"]
        self.rmct_server_port = rmct_proc["server_p"]
        self.rmct_client_ip = rmct_proc["client_ip"]
        self.rmct_client_port = rmct_proc["client_p"]

    def show_rsxa_settings(self):
        print ("server_ip=%s\nclient_ip=%s\nserver_port=%d\nclient_port=%d"%(self.rmct_server_ip, self.rmct_client_ip,
                                                                             self.rmct_server_port, self.rmct_client_port))
    def __config_multicast(self):
        """ 
        "  @brief  Configure Client/Server sockets for communication with RMCT
        """

        # -- Configure the Server ---#
        self.multi_sock_tx.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.multi_sock_tx.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_IF, socket.inet_aton(self.ip))
        self.multi_sock_tx.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, 2)
        membership_request = socket.inet_aton(self.rmct_server_ip) + socket.inet_aton(self.ip)
        self.multi_sock_tx.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, membership_request)

        # -- Configure the Client -- #
        self.multi_sock_rx.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        mreq = struct.pack("4sl", socket.inet_aton(self.rmct_client_ip), socket.INADDR_ANY)
        self.multi_sock_rx.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)
        self.multi_sock_rx.bind((self.rmct_client_ip, self.rmct_client_port))

    def tx_message(self, message):
        """ 
        "  @brief                Send Message to RMCT over the socket
        "  @param[in]            message -> Message to send
        """

        print ("Sending request to NiBot ..... {}".format(message))
        self.multi_sock_tx.sendto(message,(self.rmct_server_ip, self.rmct_server_port))

    def rx_message(self):
        """ 
        "  @brief          Recieve message from RMCT
        "  @return         Message recieved from the server
        """

        try:
            return json.loads(self.multi_sock_rx.recv(MAX_BUFF_SIZE))
        except socket.timeout:
            return False

    @staticmethod
    def construct_tx_message(motor, direction="", angle=-1, speed=-1):
        """ 
        "  @brief              Validate the arguements provided
        "  param[in] motor     Name of motor to be moved
        "  param[in] direction Direction to move in
        "  param[in] angle     Manually override motor angle
        "  param[in] speed     Set the speed of the motor
        """

        tx_message = {}
        tx_message["type"] = "hw_action"
        tx_message["motor"] = motor
        tx_message["direction"] = direction
        tx_message["angle"] = angle
        tx_message["speed"] = speed

        return json.dumps(tx_message)
