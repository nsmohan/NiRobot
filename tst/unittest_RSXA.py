#!/usr/bin/env python

"""unittest_RSXA.py:    Test all functions for the Robot Settings Library

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
RS_PATH     = "/home/nmohan/github/NiRobot/config/RSXA.json"
LIB_PATH    = "/home/nmohan/github/NiRobot/lib/"

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#
import NMT_log_test

class NMT_RSXA_test(unittest.TestCase, ):
    
    def setUp(self):
        self.rsxa = CDLL(OBJ_PATH + "libRSXA.so")
        self.backup_file = "%s.backup"%RS_PATH
        os.system("cp %s %s"%(RS_PATH, self.backup_file))
        NMT_log_test.NMT_log_test(__file__)
        self.rsxa_file_obj = self.read_rsxa_file()
        print "Running Test:", self._testMethodName
    
    def read_rsxa_file(self):
        #Read contents of current config file
        rsxa_file = open(RS_PATH, "r")
        rsxa_file_obj = rsxa_file.read()
        rsxa_file.close()
        return rsxa_file_obj

    def test_RSXA_get_mode_GW(self):
        #Description - Verify Mode is correct with customer JSON File

        #Initialize Variables
        sim_mode  = c_bool()
        OK        = 0
        test_data = [{"hw_name": "UnitTest_HW1", "hw_sim_mode": False},
                     {"hw_name": "UnitTest_HW2", "hw_sim_mode": True}]

        #Inject test_data into file
        rsxa_file_json = json.loads(self.rsxa_file_obj)
        rsxa_file_json["hw"] = test_data

        with open(RS_PATH, "w") as n_rsxa_file:
            json.dump(rsxa_file_json, n_rsxa_file)
        
        for data in test_data:
            result = self.rsxa.RSXA_get_mode(data["hw_name"], byref(sim_mode))
            self.assertEqual(data["hw_sim_mode"], sim_mode.value)
            self.assertEqual(result, OK)

    def test_RSXA_get_mode_BW(self):
        #Description - Verify Error is thrown when keys are incorrect

        #Initialize Variables
        sim_mode  = c_bool()
        NOK       = 1
        test_data = [{"hw_name": "UnitTest_HW1", "hw_sim_mode": False},
                     {"hw_name": "UnitTest_HW2", "hw_sim_mode_BW": True}]

        #Inject test_data into file
        rsxa_file_json = json.loads(self.rsxa_file_obj)
        rsxa_file_json["hw"] = test_data

        with open(RS_PATH, "w") as n_rsxa_file:
            json.dump(rsxa_file_json, n_rsxa_file)
        
        for data in test_data:
            result = self.rsxa.RSXA_get_mode(data["hw_name"], byref(sim_mode))
            self.assertEqual(result, NOK)

    def test_RSXA_get_mode_BW_blank_array(self):
        #Description - Verify Error is thrown when array is passed in is empty

        #Initialize Variables
        sim_mode  = c_bool()
        NOK       = 1
        test_data = []
        hw_name       = "UnitTest_HW3"

        #Inject test_data into file
        rsxa_file_json = json.loads(self.rsxa_file_obj)
        rsxa_file_json["hw"] = test_data

        with open(RS_PATH, "w") as n_rsxa_file:
            json.dump(rsxa_file_json, n_rsxa_file)
        
        result = self.rsxa.RSXA_get_mode(hw_name, byref(sim_mode))
        self.assertEqual(result, NOK)

    def tearDown(self):
        os.system("cp %s %s"%(self.backup_file, RS_PATH))
        os.system("rm -rf %s"%self.backup_file)

if __name__ == '__main__':
    unittest.main()

