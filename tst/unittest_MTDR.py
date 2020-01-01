#!/usr/bin/env python

"""unittest_MTDR.py:   Test all functions in MTDR

__author__             = "Nitin Mohan
__copyright__          = "Copy Right 2019. NM Technologies"
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
import sys
from ctypes import *
import unittest
import HtmlTestRunner
import os

#---------------------------------------------------#
#                   Constants                       #
#---------------------------------------------------#
OBJ_PATH    = "/home/nmohan/github/NiRobot/Obj/"
RESULT_PATH = "/home/nmohan/Test_Results/"
DAT_PATH    = "/home/nmohan/github/NiRobot/tst/dat/"  
LIB_PATH    = "/home/nmohan/github/NiRobot/lib/"

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#
sys.path.append(LIB_PATH)
from PCA9685 import PCA9685_settings
import NMT_log_test

RESULT_ENUM = ["OK", "NOK"]
LD27MG_FREQ = 50.00
FNAME = __file__.split("/")[1].split(".")[0]

sys.settrace
class MTDR_Test(unittest.TestCase):

    def setUp(self):
        self.PCA9685 = CDLL(OBJ_PATH + "libPCA9685.so")
        self.MTDR = CDLL(OBJ_PATH + "libmtdr.so")
        NMT_log_test.NMT_log_test(__file__)
        self.log_file = "%s/%s.log"%(NMT_log_test.LOG_DIR, FNAME)

    def test_MTDR_get_pca9685_status(self):

        #Description - Test the MTDR_get_pca9685_status. This will also
        #              test PCA9685_get_status function

        #Initialize Varibles
        settings = PCA9685_settings()
        initialized = c_bool()

        result = self.MTDR.MTDR_get_pca9685_status(byref(settings), byref(initialized))

        #Check if the result is OK
        self.assertEqual(result, RESULT_ENUM.index("OK"))

        self.assertEqual(False, initialized.value)
    def test_MTDR_get_current_postion(self):

        #Description - Test the MTDR_get_current_positoon. This will also
        #              test PCA9685_getPWM function

        #Initialize Varibles
        settings = PCA9685_settings()
        motor = "CAM_HRZN_MTR"
        angle = c_double()

        expected_angle = 261.76025390625 
        #Call the actual function
        result = self.MTDR.MTDR_get_current_position(motor, byref(angle), byref(settings))

        #Check if the result is OK
        self.assertEqual(result, RESULT_ENUM.index("OK"))

        #Check Actual vs Expected
        self.assertEqual(expected_angle, float(angle.value))

    def test_MTDR_move_motor(self):

        #Description - Test the MTDR_move_motor function. This will also
        #              test PCA9685_setPWM function

        #Initialze
        settings = PCA9685_settings()
        motor = "CAM_HRZN_MTR"
        angle = c_double(90.00)

        #Expected Results
        expected_duty_cycle       = 5.833333
        expected_channel          = 15
        expected_tics_to_on       = "0"
        expected_tics_on_duration = "239"
        expected_tics_to_off      = "239"
        expected_channel_reg_on   = "42"
        expected_channel_reg_off  = "44"

        #Call the actual function
        result = self.MTDR.MTDR_move_motor(motor, angle, byref(settings))

        #Ensure the result is OK
        self.assertEqual(result, RESULT_ENUM.index("OK"))

        #Read Log File
        f = open(self.log_file, "r")
        log_file = f.read()
        f.close()

        #Parse the log file for contents of interest
        set_PWM_in = filter(lambda f: ("setPWM" in f) and ("> " in f), log_file.split("\n"))[0].split("> ")[1].split(" ")
        set_PWM_out = filter(lambda f: ("setPWM" in f) and ("tics_to_on" in f), log_file.split("\n"))[0].split(" ")

        for item in set_PWM_out:
            if "tics_to_on" in item:
                tics_to_on = item.split(":")[1]
            elif "tics_on_duration" in item:
                tics_on_duration = item.split(":")[1]
            elif "tics_to_off" in item:
                tics_to_off = item.split(":")[1]
            elif "channel_reg_on" in item:
                channel_reg_on = item.split(":")[1]
            elif "channel_reg_off" in item:
                channel_reg_off = item.split(":")[1]

        for item in set_PWM_in:
            if "duty_cycle" in item:
                duty_cycle = float(set_PWM_in[set_PWM_in.index(item) + 1])
            elif "channel" in item:
                channel = int(set_PWM_in[set_PWM_in.index(item) + 1])

        #Check Actual vs Expected
        self.assertEqual(duty_cycle,       expected_duty_cycle)
        self.assertEqual(channel,          expected_channel)
        self.assertEqual(tics_to_on,       expected_tics_to_on)
        self.assertEqual(tics_on_duration, expected_tics_on_duration)
        self.assertEqual(tics_to_off,      expected_tics_to_off)
        self.assertEqual(channel_reg_on,   expected_channel_reg_on)
        self.assertEqual(channel_reg_off,  expected_channel_reg_off)

    def test_MTDR_init(self):

        #Description - Test to test the MTDR_init function
        #Indirectly/Directly will test PCA9685_int, and PCA9685_setFreq

        expected_pre_scale = 121
        settings = PCA9685_settings()
        result = self.MTDR.MTDR_init(byref(settings))

        #Check that the result is OK and Frequency is as expected
        self.assertEqual(result, RESULT_ENUM.index("OK"))
        self.assertEqual(settings.freq, LD27MG_FREQ)

        #Read Log File
        f = open(self.log_file, "r")
        log_file = f.read()
        f.close()

        #Parse for Set Freq Function
        set_freq_lines = filter(lambda f: ("PCA9685_setFreq" in f) and ("< " in f), log_file.split("\n"))[0].split("< ")[1].split(" ")

        for item in set_freq_lines:
            if "pre_scale" in item:
                pre_scale = int(set_freq_lines[set_freq_lines.index(item) + 1])
                break

        #Check if the pre_scale Value Matches
        self.assertEqual(pre_scale, expected_pre_scale)


    def tearDown(self):
        os.system("rm -rf %s"%self.log_file)

if __name__ == '__main__':
    unittest.main()
