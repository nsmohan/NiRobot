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
TST_PATH = "%s/"%(os.path.dirname(os.path.realpath(__file__)))
BLD_PATH = os.path.join(TST_PATH, "bld")

PATTERNS = ["unittest_*.py", "functest_*.py"]

#---------------Start of Program ------------------#
class TestRunner(object):
    def __init__(self, args):
        self.tests_to_run = self._get_tests_to_run(args)
        self._run_tests()

    def _get_tests_to_run(self, args):
        tests_to_run = []
        file_list = os.listdir(TST_PATH)
        
        for arg in args:
            tests_to_run += fnmatch.filter(file_list, arg)

        return tests_to_run

    def _run_tests(self):
        for test in self.tests_to_run:
            print_status(test)
            os.system("python %s%s"%(TST_PATH, test))

def print_status(test):
    print("""
           ******************************************
           *                                        *
           *     RUNNING: %s              
           *                                        *
           ******************************************"""%(test))

def run_gtests():
    file_list = os.listdir(BLD_PATH)
    
    for test in file_list:
            print_status(test)
            os.system("%s/%s"%(BLD_PATH, test))

if __name__ == '__main__':

    TestRunner(PATTERNS)
    run_gtests()
    os.system("rm -rf %s/*.pyc"%TST_PATH)
