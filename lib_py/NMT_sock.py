#!/usr/bin/env python

"""
"  @file      NMT_sock.py
"  @brief     Support library for Python Socket Communication
"  @details   Library to support socket communication in python
"  @author    Nitin Mohan
"  @date      December 10, 2020
"  @copyright 2020
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
import socket
import struct

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#
#---------------------------------------------------#
#                   Global Varibles                 #
#---------------------------------------------------#
BACK_LOG = 10
MAX_BUFF_SIZE = 4096

#--------------------------------------------------------------------------#
#                               Start of Program                           #
#--------------------------------------------------------------------------#

class NMT_sock(object):

    def __init__(self, sock_type, mode, ip, port, timeout=3600):

        """ 
        "  @brief     Constructor
        "  @param[in] sock_type
        "  @param[in] mode
        "  @param[in] ip
        "  @param[in] port
        """

        #--- Initialize Class Vars --#
        self.sock_type = sock_type
        self.mode      = mode
        self.ip        = ip
        self.port      = port
        self.timeout   = timeout
        self.clients   = {}
        self.server    = None
        self.sock      = None

        #-- Initialize Sockets --#
        self.__validate_inputs()
        self.__configure_vars()
        self.__configure_sockets()

    def __del__(self):

        """ 
        "  @brief     Destructor
        """

        #-- Close Socket Connection --#
        if self.sock:
            self.sock.close()

    def __validate_inputs(self):
        
        """ 
        "  @brief Validate the input arguments for constructor
        """

        assert self.sock_type == "TCP" or self.sock_type ==  "MULTICAST", \
                "Error! Invalid sock_type provided expected: TCP || MULTI_CAST"

        assert self.mode == "SERVER" or self.mode ==  "CLIENT", \
                "Error! Invalid mode provided expected: SERVER || CLIENT"

    def __configure_vars(self):

        """ 
        "  @brief Configure the Varibles based inputs
        """

        if self.sock_type == "TCP":
            self.sock_config = socket.SOCK_STREAM  
        else:
            self.sock_config = socket.SOCK_DGRAM
            if self.mode == "SERVER":
                self.sock_multi_type = socket.IP_ADD_MEMBERSHIP
                group = socket.inet_aton(self.ip)
                self.mreq = struct.pack('4sL', group, socket.INADDR_ANY)
            else:
                self.sock_multi_type = socket.IP_MULTICAST_TTL
                self.mreq = struct.pack('b', 1)

    def __configure_sockets(self):

        """ 
        "  @brief Configure the sockets
        """

        #-- Create Socket object --#
        self.sock = socket.socket(socket.AF_INET, self.sock_config)
        self.sock.settimeout(self.timeout)

        #-- Configure sockets --#
        if self.mode == "SERVER":

            #-- Bind Socket --#
            self.sock.bind((self.ip, self.port))

            #-- Start Listener --#
            if self.sock_type == "TCP": self.sock.listen(BACK_LOG)
        else:

            #-- Connect to Server --#
            self.sock.connect((self.ip, self.port))
        if self.sock_type == "MULTICAST": 
            self.sock.setsockopt(socket.IPPROTO_IP, self.sock_multi_type, self.mreq)

    def __recv_multi_message(self):

        """ 
        "  @brief Revieve Message on Multi-Cast Socket
        "  @param[out] address
        "  @param[out] msg
        """

        #-- Rx Message --#
        msg, address = self.sock.recvfrom(MAX_BUFF_SIZE)

        #-- Exit the Function --#
        return address, msg

    def __recv_tcp_message(self):

        """ 
        "  @brief Recieve Message on TCP Socket
        "  @param[out] client
        "  @param[out] msg
        """

        if self.mode == "SERVER":
            connObj, client = self.sock.accept()

            #-- Add to client list if new client --#
            if client not in self.clients.keys():
                clients[client] = connObj

            #-- Wait for message --#
            msg = connObj.recv(MAX_BUFF_SIZE)
        else:
            msg = self.sock.recv(MAX_BUFF_SIZE)
            client = None

        #-- Exit the Function --#
        return client, msg

    def __send_multi_message(self, msg):

        """ 
        "  @brief Send Message to subscribers
        "  @param[in] msg
        """

        self.sock.sendto(msg.encode(), (self.ip, self.port))
    
    def __send_tcp_message(self, msg, client=None):

        """ 
        "  @brief Send TCP Message
        "  @param[in] client
        "  @param[in] msg
        """

        if self.mode == "SERVER":
            if client:
                clients[client].sendall(msg)
            else:
                raise "Error! No client provided!"
        else:
            self.sock.sendall(msg.encode())

    def listen(self, listener_cb=None, continous=True):

        """ 
        "  @brief Listen on Socket
        "  @param[in] listener_cb
        """

        print("Listening for Socket Connections.....")
        try:
            while True:

                if self.sock_type == "MULTICAST":
                    address, msg = self.__recv_multi_message()
                else:
                    address, msg = self.__recv_tcp_message()

                #-- Trigger Callback --#
                if listener_cb:
                    listener_cb(address, msg)
                else:
                    return address, msg

                if not continous: break

        except socket.timeout:
            print("Socket timeout occured. Stopping Listener.")
            raise

    def tx_message(self, msg, client=None):

        """ 
        "  @brief Send message on socket
        "  @param[in] client
        "  @param[in] msg
        """

        if self.sock_type == "MULTICAST":
            self.__send_multi_message(msg)
        else:
            try:
                self.__send_tcp_message(msg, client)
            except:
                raise
