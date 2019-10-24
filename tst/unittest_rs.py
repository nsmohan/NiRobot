#!/usr/bin/env python

"""unittest_rs.py:    Test all functions for the Robot Settings Library

__author__             = "Nitin Mohan
__copyright__          = "Copy Right 2019. NM Technologies"
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
import unittest
from ctypes import *

#---------------------------------------------------#
#                   Constants                       #
#---------------------------------------------------#
OBJ_PATH    = "/home/nmohan/github/NiRobot/Obj/"

class NMT_rs_test(unittest.TestCase):
    
    def setUp(self):
        self.rs = CDLL(OBJ_PATH + "librs.so")
    
    def test_rs_get_mode(self):
        self.rs.rs_get_mode("PCA9685_PWM_DRIVER", 0);

if __name__ == '__main__':
    unittest.main()

