#!/usr/bin/env python

"""run_test.py:        Executable to run all the python unit tests

__author__             = "Nitin Mohan
__copyright__          = "Copy Right 2019. NM Technologies"
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
import os
import argparse
import fnmatch

#---------------------------------------------------#
#                   Constants                       #
#---------------------------------------------------#
ROOT_PATH      = "%s"%(os.path.dirname(os.path.realpath(__file__)))[:-3]
PROC_PATH      = os.path.join(ROOT_PATH, "bld")
TOOLS_PATH     = os.path.join(ROOT_PATH, "tools")
TST_PATH       = os.path.join(ROOT_PATH, "tst")
BLD_TST_PATH   = os.path.join(ROOT_PATH, "tst/bld")
RSXA_FILE_PATH = "/etc/NiBot/RSXA.json"

PATTERNS = ["unittest_*.py", "functest_*.py"]

#---------------Start of Program ------------------#
class TestRunner(object):
    def __init__(self, args):
        self.setup()
        self.tests_to_run = self._get_tests_to_run(args)
        self._run_tests()
        self.tearDown()

    def setup(self):

        #-- Copy Needed Files --#
        os.system("cp %s/dat/RSXA.json %s"%(TST_PATH, RSXA_FILE_PATH))

        #-- Start RMCT --#
        RMCT = os.path.join(PROC_PATH, "RMCT")
        os.system(RMCT + " &")

    def tearDown(self):

        os.system("python3 %s/nibot_mtr_ctrl.py -e"%TOOLS_PATH)

    def _get_tests_to_run(self, args):
        tests_to_run = []
        file_list = os.listdir(TST_PATH)
        
        for arg in args:
            tests_to_run += fnmatch.filter(file_list, arg)

        return tests_to_run

    def _run_tests(self):
        for test in self.tests_to_run:
            print_status(test)
            os.system("python %s/%s"%(TST_PATH, test))

def print_status(test):
    print("""
           ******************************************
           *                                        *
           *     RUNNING: %s              
           *                                        *
           ******************************************"""%(test))

def run_gtests():
    file_list = os.listdir(BLD_TST_PATH)
    
    for test in file_list:
            print_status(test)
            os.system("%s/%s"%(BLD_TST_PATH, test))

if __name__ == '__main__':

    TestRunner(PATTERNS)
    run_gtests()
    os.system("rm -rf %s/*.pyc"%TST_PATH)
