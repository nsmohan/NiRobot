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
RS_PATH = os.environ["RSXA_SETTINGS"]

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#
from lib_py.RSXA import RSXA
from lib_py.RSXA import rsxa
from lib_py.NMT_stdlib_py import NMT_result
import NMT_log_test

class NMT_RSXA_test(unittest.TestCase):
    
    def setUp(self):
        self.backup_file = "%s.backup.rsxa_test"%RS_PATH
        os.system("cp %s %s"%(RS_PATH, self.backup_file))
        NMT_log_test.NMT_log_test(__file__)
        print "Running Test:", self._testMethodName
    
    def test_RSXA_init_GW(self):
        #Description - Read RSXA.json file and verify
        #              that structures are populated properly

        #Initialize Variables
        RSXA_Object = RSXA()

        #-- Initialize RSXA --#
        result = rsxa.RSXA_init(byref(RSXA_Object))
        self.assertEqual(result, NMT_result.OK)

        #-- Read Json File --#
        rsxa_file = open(RS_PATH, "r")
        rsxa_data = json.loads(rsxa_file.read())
        rsxa_file.close()

        rsxa_data.pop("_comment")
        self.verify_struct_results(rsxa_data, RSXA_Object)

    def verify_struct_results(self, json_obj, rsxa_obj):

        #Description - Parse json file and struct to compare

        for key in json_obj.keys():
            if type(json_obj[key]) is dict:
                self.verify_struct_results(json_obj[key], getattr(rsxa_obj, key))
            elif type(json_obj[key]) is list:
                for i in range(len(json_obj[key])):
                    self.verify_struct_results(json_obj[key][i], getattr(rsxa_obj, key)[i])
            else:
                self.assertEqual(json_obj[key], getattr(rsxa_obj, key))
                

    def test_RSXA_init_BW_1(self):
        #Description - Verify result is NOK if log_dir key is missing

        #Initialize Variables
        RSXA_Object = RSXA()

        # -- Prepare Test -- #
        test_data = {"log_dirs": "/test/test_file",
                     "procs"  : [{"proc_name": "UnitTest", "server_ip": "224.1.1.1",
                                  "server_p": 1000, "client_ip": "224.1.2.3", "client_p": 2000}],
                     "operational_settings" : {"cam_mtr_step_size": 10},
                     "hw": [{"hw_name": "UnitTest_HW1", "hw_sim_mode": False, 
                             "hw_interface":[{"pin_name": "p1", "pin_no": 1}]}]}

        with open(RS_PATH, "w") as n_rsxa_file:
            json.dump(test_data, n_rsxa_file)

        result = rsxa.RSXA_init(byref(RSXA_Object))
        self.assertEqual(result, NMT_result.NOK)

    def test_RSXA_init_BW_2(self):
        #Description - Verify result is NOK if hw_name is missing

        #Initialize Variables
        RSXA_Object = RSXA()

        # -- Prepare Test -- #
        test_data = {"log_dir": "/test/test_file",
                     "procs"  : [{"proc_name": "UnitTest", "server_ip": "224.1.1.1",
                                  "server_p": 1000, "client_ip": "224.1.2.3", "client_p": 2000}],
                     "operational_settings" : {"cam_mtr_step_size": 10,
                                               "default_drive_speed": 50},
                     "hw": [{"hw_names": "UnitTest_HW1", "hw_sim_mode": False, 
                             "hw_interface":[{"pin_name": "p1", "pin_no": 1}]}]} 

        with open(RS_PATH, "w") as n_rsxa_file:
            json.dump(test_data, n_rsxa_file)

        result = rsxa.RSXA_init(byref(RSXA_Object))
        self.assertEqual(result, NMT_result.NOK)

    def test_RSXA_init_BW_3(self):
        #Description - Verify result is NOK if hw_sim_mode is missing

        #Initialize Variables
        RSXA_Object = RSXA()

        # -- Prepare Test -- #
        test_data = {"log_dir": "/test/test_file",
                     "procs"  : [{"proc_name": "UnitTest", "server_ip": "224.1.1.1",
                                  "server_p": 1000, "client_ip": "224.1.2.3", "client_p": 2000}],
                     "operational_settings" : {"cam_mtr_step_size": 10},
                     "hw": [{"hw_name": "UnitTest_HW1", "hw_sim_modes": False, 
                             "hw_interface":[{"pin_name": "p1", "pin_no": 1}]}]} 

        with open(RS_PATH, "w") as n_rsxa_file:
            json.dump(test_data, n_rsxa_file)

        result = rsxa.RSXA_init(byref(RSXA_Object))
        self.assertEqual(result, NMT_result.NOK)

    def test_RSXA_init_BW_4(self):
        #Description - Verify result is NOK if hw_interface is missing

        #Initialize Variables
        RSXA_Object = RSXA()

        # -- Prepare Test -- #
        test_data = {"log_dir": "/test/test_file",
                     "procs"  : [{"proc_name": "UnitTest", "server_ip": "224.1.1.1",
                                  "server_p": 1000, "client_ip": "224.1.2.3", "client_p": 2000}],
                     "operational_settings" : {"cam_mtr_step_size": 10,
                                               "default_drive_speed": 50},
                     "hw": [{"hw_name": "UnitTest_HW1", "hw_sim_mode": False, 
                             "hw_interfaces":[{"pin_name": "p1", "pin_no": 1}]}]} 

        with open(RS_PATH, "w") as n_rsxa_file:
            json.dump(test_data, n_rsxa_file)

        result = rsxa.RSXA_init(byref(RSXA_Object))
        self.assertEqual(result, NMT_result.NOK)

    def test_RSXA_init_BW_5(self):
        #Description - Verify result is NOK if pin_names is missing

        #Initialize Variables
        RSXA_Object = RSXA()

        # -- Prepare Test -- #
        test_data = {"log_dir": "/test/test_file",
                     "procs"  : [{"proc_name": "UnitTest", "server_ip": "224.1.1.1",
                                  "server_p": 1000, "client_ip": "224.1.2.3", "client_p": 2000}],
                     "operational_settings" : {"cam_mtr_step_size": 10,
                                               "default_drive_speed": 50},
                     "hw": [{"hw_name": "UnitTest_HW1", "hw_sim_mode": False, 
                             "hw_interface":[{"pin_names": "p1", "pin_no": 1}]}]} 

        with open(RS_PATH, "w") as n_rsxa_file:
            json.dump(test_data, n_rsxa_file)

        result = rsxa.RSXA_init(byref(RSXA_Object))
        self.assertEqual(result, NMT_result.NOK)

    def test_RSXA_init_BW_5(self):
        #Description - Verify result is NOK if pin_no is missing

        #Initialize Variables
        RSXA_Object = RSXA()

        # -- Prepare Test -- #
        test_data = {"log_dir": "/test/test_file",
                     "procs"  : [{"proc_name": "UnitTest", "server_ip": "224.1.1.1",
                                  "server_p": 1000, "client_ip": "224.1.2.3", "client_p": 2000}],
                     "operational_settings" : {"cam_mtr_step_size": 10,
                                               "default_drive_speed": 50},
                     "hw": [{"hw_name": "UnitTest_HW1", "hw_sim_mode": False, 
                             "hw_interface":[{"pin_name": "p1", "pin_nos": 1}]}]} 

        with open(RS_PATH, "w") as n_rsxa_file:
            json.dump(test_data, n_rsxa_file)

        result = rsxa.RSXA_init(byref(RSXA_Object))
        self.assertEqual(result, NMT_result.NOK)

    def test_RSXA_init_BW_6(self):
        #Description - Verify result is NOK if procs has bad values

        #Initialize Variables
        RSXA_Object = RSXA()

        # -- Prepare Test -- #
        test_data = {"log_dir": "/test/test_file",
                     "procss"  : [{"proc_name": "UnitTest", "server_ip": "224.1.1.1",
                                  "server_p": 1000, "client_ip": "224.1.2.3", "client_p": 2000}],
                     "operational_settings" : {"cam_mtr_step_size": 10,
                                               "default_drive_speed": 50},
                     "hw": [{"hw_name": "UnitTest_HW1", "hw_sim_mode": False, 
                             "hw_interface":[{"pin_name": "p1", "pin_nos": 1}]}]} 

        with open(RS_PATH, "w") as n_rsxa_file:
            json.dump(test_data, n_rsxa_file)

        result = rsxa.RSXA_init(byref(RSXA_Object))
        self.assertEqual(result, NMT_result.NOK)

    def test_RSXA_init_BW_7(self):
        #Description - Verify result is NOK if proc_name has bad values

        #Initialize Variables
        RSXA_Object = RSXA()

        # -- Prepare Test -- #
        test_data = {"log_dir": "/test/test_file",
                     "procs"  : [{"proc_names": "UnitTest", "server_ip": "224.1.1.1",
                                  "server_p": 1000, "client_ip": "224.1.2.3", "client_p": 2000}],
                     "operational_settings" : {"cam_mtr_step_size": 10,
                                               "default_drive_speed": 50},
                     "hw": [{"hw_name": "UnitTest_HW1", "hw_sim_mode": False, 
                             "hw_interface":[{"pin_name": "p1", "pin_nos": 1}]}]} 

        with open(RS_PATH, "w") as n_rsxa_file:
            json.dump(test_data, n_rsxa_file)

        result = rsxa.RSXA_init(byref(RSXA_Object))
        self.assertEqual(result, NMT_result.NOK)

    def test_RSXA_init_BW_8(self):
        #Description - Verify result is NOK if server_ip has bad values

        #Initialize Variables
        RSXA_Object = RSXA()

        # -- Prepare Test -- #
        test_data = {"log_dir": "/test/test_file",
                     "procs"  : [{"proc_name": "UnitTest", "server_ips": "224.1.1.1",
                                  "server_p": 1000, "client_ip": "224.1.2.3", "client_p": 2000}],
                     "operational_settings" : {"cam_mtr_step_size": 10,
                                               "default_drive_speed": 50},
                     "hw": [{"hw_name": "UnitTest_HW1", "hw_sim_mode": False, 
                             "hw_interface":[{"pin_name": "p1", "pin_nos": 1}]}]} 

        with open(RS_PATH, "w") as n_rsxa_file:
            json.dump(test_data, n_rsxa_file)

        result = rsxa.RSXA_init(byref(RSXA_Object))
        self.assertEqual(result, NMT_result.NOK)

    def test_RSXA_init_BW_9(self):
        #Description - Verify result is NOK if server_p has bad values

        #Initialize Variables
        RSXA_Object = RSXA()

        # -- Prepare Test -- #
        test_data = {"log_dir": "/test/test_file",
                     "procs"  : [{"proc_name": "UnitTest", "server_ip": "224.1.1.1",
                                  "server_ps": 1000, "client_ip": "224.1.2.3", "client_p": 2000}],
                     "operational_settings" : {"cam_mtr_step_size": 10,
                                               "default_drive_speed": 50},
                     "hw": [{"hw_name": "UnitTest_HW1", "hw_sim_mode": False, 
                             "hw_interface":[{"pin_name": "p1", "pin_nos": 1}]}]} 

        with open(RS_PATH, "w") as n_rsxa_file:
            json.dump(test_data, n_rsxa_file)

        result = rsxa.RSXA_init(byref(RSXA_Object))
        self.assertEqual(result, NMT_result.NOK)

    def test_RSXA_init_BW_10(self):
        #Description - Verify result is NOK if client_ip has bad values

        #Initialize Variables
        RSXA_Object = RSXA()

        # -- Prepare Test -- #
        test_data = {"log_dir": "/test/test_file",
                     "procs"  : [{"proc_name": "UnitTest", "server_ip": "224.1.1.1",
                                  "server_p": 1000, "client_ips": "224.1.2.3", "client_p": 2000}],
                     "operational_settings" : {"cam_mtr_step_size": 10,
                                               "default_drive_speed": 50},
                     "hw": [{"hw_name": "UnitTest_HW1", "hw_sim_mode": False, 
                             "hw_interface":[{"pin_name": "p1", "pin_nos": 1}]}]} 

        with open(RS_PATH, "w") as n_rsxa_file:
            json.dump(test_data, n_rsxa_file)

        result = rsxa.RSXA_init(byref(RSXA_Object))
        self.assertEqual(result, NMT_result.NOK)

    def test_RSXA_init_BW_11(self):
        #Description - Verify result is NOK if client_p has bad values

        #Initialize Variables
        RSXA_Object = RSXA()

        # -- Prepare Test -- #
        test_data = {"log_dir": "/test/test_file",
                     "procs"  : [{"proc_name": "UnitTest", "server_ip": "224.1.1.1",
                                  "server_p": 1000, "client_ip": "224.1.2.3", "client_ps": 2000}],
                     "operational_settings" : {"cam_mtr_step_size": 10,
                                               "default_drive_speed": 50},
                     "hw": [{"hw_name": "UnitTest_HW1", "hw_sim_mode": False, 
                             "hw_interface":[{"pin_name": "p1", "pin_nos": 1}]}]} 

        with open(RS_PATH, "w") as n_rsxa_file:
            json.dump(test_data, n_rsxa_file)

        result = rsxa.RSXA_init(byref(RSXA_Object))
        self.assertEqual(result, NMT_result.NOK)

    def test_RSXA_init_BW_Bad_File(self):
        #Description - Verify RSXA Init fails if Settings file is incorrect

        #Initialize Variables
        RSXA_Object = RSXA()

        #-- Remove RSXA Json File --#
        os.system("rm -rf %s"%RS_PATH)

        result = rsxa.RSXA_init(byref(RSXA_Object))
        self.assertEqual(result, NMT_result.NOK)

    def tearDown(self):
        os.system("cp %s %s"%(self.backup_file, RS_PATH))
        os.system("rm -rf %s"%self.backup_file)

if __name__ == '__main__':
    unittest.main()

