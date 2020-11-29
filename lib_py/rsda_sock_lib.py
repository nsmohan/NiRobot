#!/usr/bin/env python

"""
"  @file      rsda_sock_lib.py
"  @brief     Library to facilitate communication with RSDA Proc
"  @details   Interfaces provided for communication with RSDA
"  @author    Nitn Mohan
"  @date      November 26, 2020
"  @copyright 2020
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

#---------------------------------------------------#
#                   Global Varibles                 #
#---------------------------------------------------#
""" @var RSXA_FILE Path to RSXA.json file """
RSXA_FILE = "/etc/NiBot/RSXA.json"

RSDA = "RSDA"
MAX_BUFF_SIZE = 4096

#--------------------------------------------------------------------------#
#                               Start of Program                           #
#--------------------------------------------------------------------------#

class RSDASockConnect(object):

    def __init__(self, ip_address=""):
        self.__rsxa_settings(ip_address)
        self.__create_and_connect_socket()
    
    #---------------------------------------------------#
    #                   Private Methods                 #
    #---------------------------------------------------#
    def __del__(self):
        self.server_sock.close()

    def __create_and_connect_socket(self):

        """ 
        "  @brief  Construct Multi-Cast Socket
        """

        self.server_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.server_sock.bind(('', self.rsda_client_port))

        group = socket.inet_aton(self.rsda_client_ip)
        mreq = struct.pack('4sL', group, socket.INADDR_ANY)
        self.server_sock.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)
        
    def __rsxa_settings(self, ip_address):

        """ 
        "  @brief  Read the Robot Settings file and get needed settings
        """

        # -- Read the File -- #
        with open(RSXA_FILE, "r") as rsxa_file:
            rsxa = json.load(rsxa_file)

        # -- Find the Settings Needed -- #
        rmct_proc = list(filter(lambda p: p["proc_name"] == RSDA, rsxa["procs"]))[0]
        
        # -- Load the Settings into the object -- #
        if ip_address:
            # -- Override IP Address --#
            self.rmct_client_ip = ip_address
        else:
            self.rsda_client_ip = rmct_proc["client_ip"]
        self.rsda_client_port = rmct_proc["client_p"]

if __name__ == '__main__':

    rsda = RSDASockConnect()
    data, address = rsda.server_sock.recvfrom(1024)

    print (json.loads(data))

