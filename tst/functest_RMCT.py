#!/usr/bin/env python

"""
"  @file      functest_RMCT.py
"  @brief     Functional Tests for the RMCT Process
"  @details   Send messages to RMCT and validate results
"  @author    Nitin Mohan
"  @date      April 7, 2020
"  @copyright 2020 - NM Technologies
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
import unittest
import os
import time

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#
from lib_py.rmct_sock_lib import RMCTSockConnect
from lib_py.rmct_sock_lib import NMT_result
from lib_py import NMT_log_parse

#---------------------------------------------------#
#                   Constants                       #
#---------------------------------------------------#
LOG_DIR  = "/var/log/NiRobot"
ROOT       = os.getcwd().split("NiRobot")[0]
TOOLS_PATH = os.path.join(ROOT, "NiRobot", "tools")
DAT_PATH = os.path.join(ROOT, "NiRobot", "tst", "dat")
CLR_LOGS   = os.path.join(TOOLS_PATH, "clear_logs.sh")
BASE_FILE = "rmct_base.log"

# ---- Start of Tests -----#
class MTDR_Test(unittest.TestCase):

    def setUp(self):
        os.system("sudo %s"%CLR_LOGS)
        self.rmct = RMCTSockConnect()

    def test_RMCT(self):
        camera = "CAMERA"
        camera_motors = ["CAM_HRZN_MTR", "CAM_VERT_MTR"]
        drive_motors = ["LEFT_DRV_MTR", "RIGHT_DRV_MTR"]

        cam_directions = ["UP", "DOWN", "LEFT", "RIGHT"]
        drv_directions = ["FORWARD", "REVERSE", "STOP"]

        angle = 45
        speed = 30

        # -- Send Message with Camera Directions -- #
        for direction in cam_directions:
            message = self.rmct.construct_tx_message(camera, direction)
            self.rmct.tx_message(message)
            rx_message = self.rmct.rx_message()
            self.assertEqual("ack", rx_message["type"])
            self.assertEqual(NMT_result.OK, rx_message["result"])
            time.sleep(0.5)

        # -- Specify Custom Angle for motors --#
        for motor in camera_motors:
            message = self.rmct.construct_tx_message(motor, angle=angle)
            self.rmct.tx_message(message)
            rx_message = self.rmct.rx_message()
            self.assertEqual("ack", rx_message["type"])
            self.assertEqual(NMT_result.OK, rx_message["result"])
            time.sleep(0.5)

        # -- Test with Drive Motor -- #
        for drv_motor in drive_motors:
            for direction in drv_directions:
                message = self.rmct.construct_tx_message(drv_motor, direction=direction, speed=speed)
                self.rmct.tx_message(message)
                rx_message = self.rmct.rx_message()
                self.assertEqual("ack", rx_message["type"])
                self.assertEqual(NMT_result.OK, rx_message["result"])
                time.sleep(0.5)

        # -- Negative Scenario -- #
        message = self.rmct.construct_tx_message(camera)
        self.rmct.tx_message(message)
        rx_message = self.rmct.rx_message()
        self.assertEqual("ack", rx_message["type"])
        self.assertEqual(NMT_result.NOK, rx_message["result"])

        self.validate_logs()

    def validate_logs(self):
        test_run_log = NMT_log_parse.parse_log_data(LOG_DIR, "RMCT.log")
        base_log = NMT_log_parse.parse_log_data(DAT_PATH, BASE_FILE)

        self.assertEqual(len(test_run_log),len(base_log))

        for base, test in zip(base_log, test_run_log):
            self.assertEqual(base["method"], test["method"], msg="ERROR: %s does not equal %s"%(base["method"], test["method"]))
            self.assertEqual(base["message"], test["message"], msg="ERROR: %s does not equal %s"%(base["message"], test["message"]))
            
    def tearDown(self):
        pass
        #os.system("sudo %s"%CLR_LOGS)

if __name__ == '__main__':
    unittest.main()
