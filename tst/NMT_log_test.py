#!/usr/bin/env python

"""NMT_log_test.py:    Supporting file to handle logging for tests

__author__             = "Nitin Mohan
__copyright__          = "Copy Right 2019. NM Technologies"
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
from ctypes import *

#---------------------------------------------------#
#                   Constants                       #
#---------------------------------------------------#
LOG_DIR     = "/var/log/NiRobot"

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#

#---------------- Start of Program ------------------#
class NMT_log_test(object):

    def __init__(self, file_name):
        self.NMT_log   = CDLL("Obj/libNMT_log.so")
        self.log_dir   = LOG_DIR
        self.NMT_log.NMT_log_init_m(file_name, self.log_dir, False)
