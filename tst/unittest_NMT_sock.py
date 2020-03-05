#!/usr/bin/env python

"""unittest_NMT_sock.py:    

__author__             = "Nitin Mohan
__copyright__          = "Copy Right 2019. NM Technologies"
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
from ctypes import *
import socket
import struct
import unittest

#---------------------------------------------------#
#                   Constants                       #
#---------------------------------------------------#
LOG_DIR     = "/var/log/NiRobot"
MULTICAST_PORT = 5500
MULTICAST_IP   = "224.1.1.1"
FNAME = __file__.split(".")[0].split("/")[-1]

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#
import NMT_log_test
from Obj import libNMT_sock as NMT_sock
from Obj.libNMT_sock import NMT_result
from Obj.libNMT_sock import sock_mode

#---------------- Start of Program ------------------#
#sock_multi = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
#sock_multi.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
#sock_multi.bind((MULTICAST_IP, MULTICAST_PORT))
#mreq = struct.pack("4sl", socket.inet_aton(MULTICAST_IP), socket.INADDR_ANY)
#sock_multi.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)

#while True:
#    data = sock_multi.recv(10240)
#    print data

class MTDR_Test(unittest.TestCase):

    def setUp(self):
        NMT_log_test.NMT_log_test(__file__)
        self.log_file = "%s/%s.log"%(NMT_log_test.LOG_DIR, FNAME)

    def test_01(self):
        server_object = NMT_sock.NMT_sock_multicast(MULTICAST_PORT, MULTICAST_IP, sock_mode.SOCK_SERVER)
        self.assertEqual(server_object.NMT_get_result(), NMT_result.OK)

        client_object = NMT_sock.NMT_sock_multicast(MULTICAST_PORT, MULTICAST_IP, sock_mode.SOCK_CLIENT)
        self.assertEqual(client_object.NMT_get_result(), NMT_result.OK)

        result = server_object.NMT_write_socket("This is a test message")
        self.assertEqual(result, NMT_result.OK)

        result = client_object.NMT_read_socket()
        self.assertEqual(result, NMT_result.OK)


if __name__ == '__main__':
    unittest.main()
