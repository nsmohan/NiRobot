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
        #Description - Create custom RSXA.json file and confrim
        #              that structures are populated properly

        #Initialize Variables
        RSXA_Object = RSXA()

        # -- Prepare Test -- #
        test_data = {"log_dir": "/test/test_file",
                     "procs"  : [{"proc_name": "UnitTest", "server_ip": "224.1.1.1",
                                  "server_p": 1000, "client_ip": "224.1.2.3", "client_p": 2000}],
                     "operational_settings" : {"cam_mtr_step_size"  : 10,
                                               "default_drive_speed": 50,
                                               "rsda_broadcast_freq": 100},
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

        result = rsxa.RSXA_init(byref(RSXA_Object))
        self.assertEqual(result, NMT_result.OK)

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

