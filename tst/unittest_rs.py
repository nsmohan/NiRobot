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
import os
import json

#---------------------------------------------------#
#                   Constants                       #
#---------------------------------------------------#
OBJ_PATH    = "/home/nmohan/github/NiRobot/Obj/"
RS_PATH     = "/home/nmohan/github/NiRobot/config/rs.json"
LIB_PATH    = "/home/nmohan/github/NiRobot/lib/"

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#
import NMT_log_test

class NMT_rs_test(unittest.TestCase, ):
    
    def setUp(self):
        self.rs = CDLL(OBJ_PATH + "librs.so")
        self.backup_file = "%s.backup"%RS_PATH
        os.system("cp %s %s"%(RS_PATH, self.backup_file))
        NMT_log_test.NMT_log_test(__file__)
    
    def test_rs_get_mode(self):
        #Description - Verify Mode is correct with customer JSON File

        #Initialize Variables
        sim_mode  = c_bool()
        test_data = [{"hw_name": "UnitTest_HW1", "hw_sim_mode": False},
                     {"hw_name": "UnitTest_HW2", "hw_sim_mode": True}]
        
        #Read contents of current config file
        rs_file = open(RS_PATH, "r")
        rs_file_obj = rs_file.read()
        rs_file.close()

        #Inject test_data into file
        rs_file_json = json.loads(rs_file_obj)
        rs_file_json["hw"] = test_data

        with open(RS_PATH, "w") as n_rs_file:
            json.dump(rs_file_json, n_rs_file)
        
        for data in test_data:
            result = self.rs.rs_get_mode(data["hw_name"], byref(sim_mode))
            #self.assertEqual(data["hw_sim_mode"], sim_mode.value)
            print result

    def tearDown(self):
        os.system("cp %s %s"%(self.backup_file, RS_PATH))
        os.system("rm -rf %s"%self.backup_file)

if __name__ == '__main__':
    unittest.main()

