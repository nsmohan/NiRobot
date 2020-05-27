#!/usr/bin/env python

"""unittest_NMT_sock.py:    

__author__             = "Nitin Mohan
__copyright__          = "Copy Right 2019. NM Technologies"
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
from ctypes import *
import unittest

#---------------------------------------------------#
#                   Constants                       #
#---------------------------------------------------#
MULTICAST_PORT = 5500
MULTICAST_IP   = "224.3.29.71"
TIMEOUT       = 1
FNAME = __file__.split(".")[0].split("/")[-1]

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#
import NMT_log_test
from Obj import NMT_sock
from Obj.NMT_sock import NMT_result
from Obj.NMT_sock import sock_mode

#---------------- Start of Program ------------------#

class NMT_sock_Test(unittest.TestCase):

    def setUp(self):
        NMT_log_test.NMT_log_test(__file__)
        self.log_file = "%s/%s.log"%(NMT_log_test.LOG_DIR, FNAME)

    def test_GWTest_tx_rx(self):

        ##
         # @test Test that client is able to recieve data that the server sends
         # @step 1. Create 3 instances of the socket object (Server and 2 Clients)
         # @step 2. Server sends messages on socket
         # @step 3. Client is able to recieve the message that was sent by the server
         #

        #@var Test messaget that is sent 
        message_tx = "This is a test message"

        # - Create instnace of the server object and check the result is ok
        server_object = NMT_sock.NMT_sock_multicast(MULTICAST_PORT, MULTICAST_IP, sock_mode.SOCK_SERVER, 1)
        self.assertEqual(server_object.NMT_get_result(), NMT_result.OK)

        # - Create instance of the first client object
        client_object = NMT_sock.NMT_sock_multicast(MULTICAST_PORT, MULTICAST_IP, sock_mode.SOCK_CLIENT, 1)
        self.assertEqual(client_object.NMT_get_result(), NMT_result.OK)

        # Create instance of the second client object
        client_object2 = NMT_sock.NMT_sock_multicast(MULTICAST_PORT, MULTICAST_IP, sock_mode.SOCK_CLIENT, 1)
        self.assertEqual(client_object2.NMT_get_result(), NMT_result.OK)

        # Write Message to socket and check the result
        result = server_object.NMT_write_socket(message_tx)
        self.assertEqual(result, NMT_result.OK)

        #Read message of the first client socket
        result, message_rx = client_object.NMT_read_socket()
        self.assertEqual(result, NMT_result.OK)

        #Read message on the second client socket
        result2, message_rx2 = client_object2.NMT_read_socket()
        self.assertEqual(result2, NMT_result.OK)

        # Data Validation
        print ("Message from first client socket=%s"%message_rx)
        print ("Message from second client socket=%s"%message_rx2)
        self.assertEqual(message_tx, message_rx)
        self.assertEqual(message_tx, message_rx2)

    def test_BWClientTimeOut(self):

        ##
         # @test Test scenario where no message is recieved on the socket
         # @step 1. Create 2 instances of the socket object (Server and Client)
         # @step 2. Client listens on the socket for 100ms and returns NOk as nothing is recieved
         #

        # - Create instance of socket client
        client_object = NMT_sock.NMT_sock_multicast(MULTICAST_PORT, MULTICAST_IP, sock_mode.SOCK_CLIENT, 1)
        self.assertEqual(client_object.NMT_get_result(), NMT_result.OK)

        #Read message on socket
        result, message_rx = client_object.NMT_read_socket()
        self.assertEqual(result, NMT_result.NOK)

        #Message should be empty
        self.assertEqual("", message_rx)

    def test_BWTestPortMismatch(self):

        ##
         # @test Test Client does not rx anything when it joins with the incorrect port number
         # @step 1. Create 2 instances Server and Client (Client subscribes to incorrect port)
         # @step 2. Send Message
         # @step 3. Client timesout and does not recieve the message sent by the server
         #

        #@var Test messaget that is sent 
        message_tx = "This is a test message"
        wrong_port = MULTICAST_PORT + 1

        # - Create instnace of the server object and check the result is ok
        server_object = NMT_sock.NMT_sock_multicast(MULTICAST_PORT, MULTICAST_IP, sock_mode.SOCK_SERVER, 1)
        self.assertEqual(server_object.NMT_get_result(), NMT_result.OK)

        # - Create instance of the first client object
        client_object = NMT_sock.NMT_sock_multicast(wrong_port, MULTICAST_IP, sock_mode.SOCK_CLIENT, 1)
        self.assertEqual(client_object.NMT_get_result(), NMT_result.OK)

        # Write Message to socket and check the result
        result = server_object.NMT_write_socket(message_tx)
        self.assertEqual(result, NMT_result.OK)

        #Read message of the first client socket
        result, message_rx = client_object.NMT_read_socket()
        self.assertEqual(result, NMT_result.NOK)

        # Data Validation
        print ("Message from first client socket=%s"%message_rx)
        self.assertEqual("", message_rx)

if __name__ == '__main__':
    unittest.main()
