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
import sys

#---------------------------------------------------#
#                   Constants                       #
#---------------------------------------------------#
RS_PATH     = "/etc/NiBot/RSXA.json"
OK = 0
NOK = 1

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#
from RSXA import RSXA_hw
import NMT_log_test

class NMT_RSXA_test(unittest.TestCase, ):
    
    def setUp(self):
        self.rsxa = CDLL("Obj/libRSXA.so")
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
        hw_settings_obj = RSXA_hw()

        # -- Prepare Test -- #
        test_data = [{"hw_name": "UnitTest_HW1", "hw_sim_mode": False},
                     {"hw_name": "UnitTest_HW2", "hw_sim_mode": True}]

        #Inject test_data into file
        rsxa_file_json = json.loads(self.rsxa_file_obj)
        rsxa_file_json["hw"] = test_data

        with open(RS_PATH, "w") as n_rsxa_file:
            json.dump(rsxa_file_json, n_rsxa_file)


        # -- Start of Actual Test -- #

        #Initialze RSXA
        result = self.rsxa.RSXA_init(byref(hw_settings_obj))
        self.assertEqual(result, OK)

        #Verify Sim mode by RSXA_get_mode
        for data in test_data:
            result = self.rsxa.RSXA_get_mode(data["hw_name"], byref(sim_mode), hw_settings_obj)
            self.assertEqual(data["hw_sim_mode"], sim_mode.value)
            self.assertEqual(result, OK)

    def test_RSXA_get_mode_BW(self):
        #Description - Verify Error is thrown when keys are incorrect

        #Initialize Variables
        sim_mode  = c_bool()
        hw_settings_obj = RSXA_hw()

        test_data = [{"hw_name": "UnitTest_HW1", "hw_sim_mode": False},
                     {"hw_name": "UnitTest_HW2", "hw_sim_mode_BW": True}]

        #Inject test_data into file
        rsxa_file_json = json.loads(self.rsxa_file_obj)
        rsxa_file_json["hw"] = test_data

        with open(RS_PATH, "w") as n_rsxa_file:
            json.dump(rsxa_file_json, n_rsxa_file)

        #Initialze RSXA and confirm result is NOK file has invalid key
        result = self.rsxa.RSXA_init(byref(hw_settings_obj))
        self.assertEqual(result, NOK)

    def test_RSXA_get_mode_BW_blank_array(self):
        #Description - Verify Error is thrown when array is passed in is empty

        #Initialize Variables
        hw_settings_obj = RSXA_hw()
        sim_mode  = c_bool()
        test_data = []
        hw_name       = "UnitTest_HW3"

        #Inject test_data into file
        rsxa_file_json = json.loads(self.rsxa_file_obj)
        rsxa_file_json["hw"] = test_data

        with open(RS_PATH, "w") as n_rsxa_file:
            json.dump(rsxa_file_json, n_rsxa_file)
        
        #Initialze RSXA -> Result is NOK as the array is blank
        result = self.rsxa.RSXA_init(byref(hw_settings_obj))
        self.assertEqual(result, NOK)

        #Verify Sim mode by RSXA_get_mode
        result = self.rsxa.RSXA_get_mode(hw_name, byref(sim_mode), hw_settings_obj)
        self.assertEqual(result, NOK)

    def tearDown(self):
        os.system("cp %s %s"%(self.backup_file, RS_PATH))
        os.system("rm -rf %s"%self.backup_file)

if __name__ == '__main__':
    unittest.main()

