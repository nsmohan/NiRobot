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
TST_PATH    = "/home/nmohan/github/NiRobot/tst/"

#---------------Start of Program ------------------#
class TestRunner(object):
    def __init__(self, args):
        self.tests_to_run = self._get_tests_to_run(args)
        self._run_tests()

    def _get_tests_to_run(self, args):
        file_list = os.listdir(TST_PATH)
        return fnmatch.filter(file_list, args)

    def _run_tests(self):
        for test in self.tests_to_run:
            print "Running ..........python %s%s"%(TST_PATH, test)
            os.system("python %s%s"%(TST_PATH, test))


if __name__ == '__main__':

    #Parse arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('-p', '--pattern', required = True,  help = "-p/--pattern Enter test pattern")
    args = parser.parse_args()

    TestRunner(args.pattern)
