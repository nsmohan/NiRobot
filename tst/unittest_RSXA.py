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
from lib_py.RSXA import RSXA
import NMT_log_test

class NMT_RSXA_test(unittest.TestCase):
    
    def setUp(self):
        self.rsxa = CDLL("Obj/libRSXA.so")
        self.backup_file = "%s.backup"%RS_PATH
        os.system("cp %s %s"%(RS_PATH, self.backup_file))
        NMT_log_test.NMT_log_test(__file__)
        print "Running Test:", self._testMethodName
    
    def test_RSXA_init_GW(self):
        #Description - Create custom RSXA.json file and confrim
        #              that structures are populated properly

        #Initialize Variables
        RSXA_Object = RSXA()

        # -- Prepare Test -- #
        test_data = {"log_dir": "/test/test_file",
                     "hw": [{"hw_name": "UnitTest_HW1", "hw_sim_mode": False, 
                             "hw_interface":[{"pin_name": "p1", "pin_no": 1}, 
                                             {"pin_name": "p2", "pin_no": 2}]},
                            {"hw_name": "UnitTest_HW2", "hw_sim_mode": True,
                             "hw_interface": [{"pin_name": "p3", "pin_no": 3}, 
                                              {"pin_name": "p4", "pin_no": 4}]},
                            {"hw_name": "UnitTest_HW2", "hw_sim_mode": True,
                             "hw_interface": []}]}

        with open(RS_PATH, "w") as n_rsxa_file:
            json.dump(test_data, n_rsxa_file)

        result = self.rsxa.RSXA_init(byref(RSXA_Object))
        self.assertEqual(result, OK)

        # Check log_dir
        self.assertEqual(test_data["log_dir"], RSXA_Object.log_dir)

        # Check hw structure 
        for i in range(0, len(test_data["hw"])):
            self.assertEqual(len(test_data["hw"]), RSXA_Object.array_len_hw)
            self.assertEqual(test_data["hw"][i]["hw_name"], RSXA_Object.hw[i].hw_name)
            self.assertEqual(test_data["hw"][i]["hw_sim_mode"], RSXA_Object.hw[i].hw_sim_mode)
            for j in range(0, len(test_data["hw"][i]["hw_interface"])):
                self.assertEqual(len(test_data["hw"][i]["hw_interface"]),
                                 RSXA_Object.hw[i].array_len_hw_int)
                self.assertEqual(test_data["hw"][i]["hw_interface"][j]["pin_name"],
                                 RSXA_Object.hw[i].hw_interface[j].pin_name)
                self.assertEqual(test_data["hw"][i]["hw_interface"][j]["pin_no"], 
                                 RSXA_Object.hw[i].hw_interface[j].pin_no)

    def test_RSXA_init_BW_1(self):
        #Description - Verify result is NOK if log_dir key is missing

        #Initialize Variables
        RSXA_Object = RSXA()

        # -- Prepare Test -- #
        test_data = {"log_dirs": "/test/test_file",
                     "hw": [{"hw_name": "UnitTest_HW1", "hw_sim_mode": False, 
                             "hw_interface":[{"pin_name": "p1", "pin_no": 1}]}]}

        with open(RS_PATH, "w") as n_rsxa_file:
            json.dump(test_data, n_rsxa_file)

        result = self.rsxa.RSXA_init(byref(RSXA_Object))
        self.assertEqual(result, NOK)

    def test_RSXA_init_BW_2(self):
        #Description - Verify result is NOK if hw_name is missing

        #Initialize Variables
        RSXA_Object = RSXA()

        # -- Prepare Test -- #
        test_data = {"log_dir": "/test/test_file",
                     "hw": [{"hw_names": "UnitTest_HW1", "hw_sim_mode": False, 
                             "hw_interface":[{"pin_name": "p1", "pin_no": 1}]}]} 

        with open(RS_PATH, "w") as n_rsxa_file:
            json.dump(test_data, n_rsxa_file)

        result = self.rsxa.RSXA_init(byref(RSXA_Object))
        self.assertEqual(result, NOK)

    def test_RSXA_init_BW_3(self):
        #Description - Verify result is NOK if hw_sim_mode is missing

        #Initialize Variables
        RSXA_Object = RSXA()

        # -- Prepare Test -- #
        test_data = {"log_dir": "/test/test_file",
                     "hw": [{"hw_name": "UnitTest_HW1", "hw_sim_modes": False, 
                             "hw_interface":[{"pin_name": "p1", "pin_no": 1}]}]} 

        with open(RS_PATH, "w") as n_rsxa_file:
            json.dump(test_data, n_rsxa_file)

        result = self.rsxa.RSXA_init(byref(RSXA_Object))
        self.assertEqual(result, NOK)

    def test_RSXA_init_BW_4(self):
        #Description - Verify result is NOK if hw_interface is missing

        #Initialize Variables
        RSXA_Object = RSXA()

        # -- Prepare Test -- #
        test_data = {"log_dir": "/test/test_file",
                     "hw": [{"hw_name": "UnitTest_HW1", "hw_sim_mode": False, 
                             "hw_interfaces":[{"pin_name": "p1", "pin_no": 1}]}]} 

        with open(RS_PATH, "w") as n_rsxa_file:
            json.dump(test_data, n_rsxa_file)

        result = self.rsxa.RSXA_init(byref(RSXA_Object))
        self.assertEqual(result, NOK)

    def test_RSXA_init_BW_5(self):
        #Description - Verify result is NOK if pin_names is missing

        #Initialize Variables
        RSXA_Object = RSXA()

        # -- Prepare Test -- #
        test_data = {"log_dir": "/test/test_file",
                     "hw": [{"hw_name": "UnitTest_HW1", "hw_sim_mode": False, 
                             "hw_interface":[{"pin_names": "p1", "pin_no": 1}]}]} 

        with open(RS_PATH, "w") as n_rsxa_file:
            json.dump(test_data, n_rsxa_file)

        result = self.rsxa.RSXA_init(byref(RSXA_Object))
        self.assertEqual(result, NOK)

    def test_RSXA_init_BW_5(self):
        #Description - Verify result is NOK if pin_no is missing

        #Initialize Variables
        RSXA_Object = RSXA()

        # -- Prepare Test -- #
        test_data = {"log_dir": "/test/test_file",
                     "hw": [{"hw_name": "UnitTest_HW1", "hw_sim_mode": False, 
                             "hw_interface":[{"pin_name": "p1", "pin_nos": 1}]}]} 

        with open(RS_PATH, "w") as n_rsxa_file:
            json.dump(test_data, n_rsxa_file)

        result = self.rsxa.RSXA_init(byref(RSXA_Object))
        self.assertEqual(result, NOK)

    def tearDown(self):
        os.system("cp %s %s"%(self.backup_file, RS_PATH))
        os.system("rm -rf %s"%self.backup_file)

if __name__ == '__main__':
    unittest.main()

