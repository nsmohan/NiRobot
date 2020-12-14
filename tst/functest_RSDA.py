#!/usr/bin/env python

"""
"  @file      functest_RSDA.py
"  @brief     Functional Tests for RSDA
"  @details   Blackbox testing for the RSDA process
"  @author    Nitin Mohan
"  @date      December 14, 2020
"  @copyright 2020
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
import unittest
import os
import time
import threading

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#
from lib_py.rsda_sock_lib import RSDAProc
from lib_py.NMT_stdlib_py import NMT_result

#---------------------------------------------------#
#                   Global Varibles                 #
#---------------------------------------------------#
ROOT       = os.getcwd().split("NiRobot")[0]
TOOLS_PATH = os.path.join(ROOT, "NiRobot", "tools")
CLR_LOGS   = os.path.join(TOOLS_PATH, "clear_logs.sh")

DEFAULT_SONAR_VAL = 0.1715
DEFAULT_VOLTAGE   = 6

#--------------------------------------------------------------------------#
#                               Start of Program                           #
#--------------------------------------------------------------------------#
# ---- Start of Tests -----#
class RSDA_Test(unittest.TestCase):

    def setUp(self):
        os.system("sudo %s"%CLR_LOGS)
        self.rsda = RSDAProc()
        self.result = NMT_result.OK

    def test_rsda(self):

        nrof_samples = 20

        #-- Get 20 Samples --#
        for i in range(nrof_samples):

            sensor_data = self.rsda.get_sensor_data()

            #-- Process current results --#
            validationThread = threading.Thread(target=self.__validate_results, args=(sensor_data,))
            validationThread.start()

            #-- Sleep until we get next sample --#
            time.sleep(1)
            validationThread.join()

            #-- Ensure result is OK --#
            self.assertEqual(NMT_result.OK, self.result)


    def __validate_results(self, sensor_data):

        try:
            self.assertEqual(sensor_data["Proximity"]["HCxSR04_Right"],   DEFAULT_SONAR_VAL)
            self.assertEqual(sensor_data["Proximity"]["HCxSR04_Left"],    DEFAULT_SONAR_VAL)
            self.assertEqual(sensor_data["Proximity"]["HCxSR04_Front"],   DEFAULT_SONAR_VAL)
            self.assertEqual(sensor_data["Voltage"]["PI_INPUT_VOLTAGE"],  DEFAULT_VOLTAGE)
            self.assertEqual(sensor_data["Voltage"]["BATTERY_VOLTAGE"],   DEFAULT_VOLTAGE)
        except AssertionError:
            self.result = NMT_result.NOK
            raise

    def tearDown(self):
        os.system("sudo %s"%CLR_LOGS)


if __name__ == '__main__':
    unittest.main()

