#!/usr/bin/env python

"""unittest_NMTlib.py: Test all NMT_stdlib functions

__author__             = "Nitin Mohan
__copyright__          = "Copy Right 2019. NM Technologies"
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
import sys
import os
import inspect
import unittest
import re
import json
from ctypes import *
import HtmlTestRunner

#---------------------------------------------------#
#                   Constants                       #
#---------------------------------------------------#
OBJ_PATH    = "/home/nmohan/github/NiRobot/Obj/"
RESULT_PATH = "/home/nmohan/Test_Results/"
DAT_PATH    = "/home/nmohan/github/NiRobot/tst/dat/"  

class NMT_stdlib_test(unittest.TestCase):
    
    def setUp(self):
        self.NMT_stdlib = CDLL(OBJ_PATH + "libNMT_stdlib.so")

    def test_NMT_stdlib_count(self):

        #Description - Pass various strings to function with token to count\
        #              and ensure the correct value is returned.

        #Get Function Name
        func_name = inspect.stack()[0][3] 

        #First element in tuple is string and the second is the char to count
        string_param = [("This-is-a-test-string", "-"), ("This-is-a second test string", "- ")]

        for sp in string_param: 
            no_of_param_c  = 0
            no_of_param_py = 0

            no_of_param_c  = self.NMT_stdlib.NMT_stdlib_count(sp[0], sp[1])
            for p in sp[1]:
                no_of_param_py += sp[0].count(p)

            print "%s: C-Return = %d, Py-Return = %d"%(func_name, no_of_param_c, no_of_param_py)
            self.assertEqual(no_of_param_c, no_of_param_py, 
                             msg="Expected: {}\n Actual: {}".format(no_of_param_c, no_of_param_py))

    def test_NMT_stdlib_split(self):

        #Description - Pass various strings to function with parameter to split
        #              and ensure the correct value is returned.

        #Get Function Name
        func_name = inspect.stack()[0][3] 

        #Initialize Variables
        item_array = POINTER(POINTER(c_char))()
        no_of_items = c_int()
        split_string_array = []
        split_param_struct = ""
        c_string = ""

        #Configure Inputs (First element is the string to split and the second is the deliminator"
        string_param = [("This-is-a-test-string", "-"), ("This-is-a second test string", "- ")]
        
        #Test Execution
        for sp in string_param:
            for p in sp[1]:
                split_param_struct += "%s|"%p
            split_param_struct = split_param_struct[:-1]
            split_string_array += re.split(split_param_struct, sp[0])

            self.NMT_stdlib.NMT_stdlib_split(sp[0], sp[1], byref(item_array), byref(no_of_items))
            for i in range(0, no_of_items.value):
                for j in range(0, len(split_string_array[i])):
                    c_string += item_array[i][j]
                self.assertEqual(c_string, split_string_array[i], 
                                 msg="Expected {}\n Actual: {}".format(c_string, split_string_array[i]))
                c_string = ""
            split_param_struct = ""
            split_string_array = []

    def test_NMT_stdlib_get_file_size(self):

        #Description - Pass a file to the function and
        #              the file size returned is correct


        #Initialize Variables
        file_name    = "/tmp/NMT_read_file_unittest.test"
        file_content = "This is a test file which contains a test string.\n"

        #Create File
        f = open(file_name, "w")
        f.write(file_content)
        f.close()

        #Test Execution
        file_size = self.NMT_stdlib.NMT_stdlib_get_file_size(file_name)
        self.assertEqual(file_size, len(file_content))

    def test_NMT_stdlib_read_file(self):

        #Description - Pass various strings to function with parameter to split
        #              and ensure the correct value is returned.

        #Initialize Variables
        file_name    = "/tmp/NMT_read_file_unittest.test"
        test_string  = "This is a test file which contains a test string.\n"
        c_string = ""
        file_content = POINTER(c_char)()

        #Create File
        f = open(file_name, "w")
        f.write(test_string)
        f.close()

        #Test Execution
        self.NMT_stdlib.NMT_stdlib_read_file(file_name, byref(file_content))
        for i in range(0, len(test_string)):
            c_string += file_content[i]

        self.assertEqual(c_string, test_string)

        #Clean-up
        os.system("rm -rf %s"%file_name)

    def test_NMT_stdlib_write_file(self):
        #Description - Write to a file and verify the contents
        
        #Initialize Varibles
        file_name   = "/tmp/NMT_write_file_unittest.test"
        write_string = "Testing the NMT_stdlib_write_function"

        #Invoke the function
        self.NMT_stdlib.NMT_stdlib_write_file(file_name, write_string)

        #Get Actual Results
        f = open(file_name, "r")
        read_string = f.read().rstrip()
        f.close()

        #Verify Results
        self.assertEqual(read_string, write_string)

        #Clean-up
        os.system("rm -rf %s"%file_name)

class NMT_log_Test(unittest.TestCase):

    def setUp(self):
        self.NMT_log   = CDLL(OBJ_PATH + "libNMT_log.so")
        self.log_fname = __file__.split("/")[-1].split(".")[0]
        self.log_dir   = "/tmp"
    
    def test_NMT_log_init(self):
        #Description - Test the log_init function and insure
        #              the values and correct in the log settings structure

        #Init log_settings struct in python
        log_settings_struct = logger.in_dll(self.NMT_log, 'log_settings')


        #Test 1 - Verbosity = False ? log_level == DEBUG
        self.NMT_log.NMT_log_init_m(__file__, self.log_dir, True)

        #Compare Actual vs Expected
        self.assertEqual(log_settings_struct.log_level, 0) #DEBUG = 0 
        self.assertEqual(log_settings_struct.log_dir, self.log_dir)
        self.assertEqual(log_settings_struct.file_name, self.log_fname)

        #Test 2 - Verbosity = False ? log_level == WARNING
        self.NMT_log.NMT_log_init_m(__file__, self.log_dir, False)

        #Compare Actual vs Exppected
        self.assertEqual(log_settings_struct.log_level, 1) #WARNING = 1 
        self.assertEqual(log_settings_struct.log_dir, self.log_dir)
        self.assertEqual(log_settings_struct.file_name, self.log_fname)

    def test_NMT_log_write_debug(self):
        #Description - Test the log_write function by passing different inputs
        #              and verifying that outputs are as expected

        #Init Inputs
        line_number = 10
        func_name   = "Test_Function"
        message     = "Test Message"
        log_level   = 0

        #Init log_settings struct in python
        log_settings_struct = logger.in_dll(self.NMT_log, 'log_settings')

        #Test 1 - Verbosity = False ? log_level == DEBUG
        self.NMT_log.NMT_log_init_m(__file__, self.log_dir, True)
        self.NMT_log.NMT_log_write_m(line_number, func_name, message, 0)

        file_name = "%s/%s.log"%(self.log_dir, self.log_fname)
        f = open(file_name, "r")
        file_content = f.read().split("--")
        f.close()

        self.assertEqual(file_content[1].strip(), "DEBUG")
        self.assertEqual(file_content[2].strip(), self.log_fname)
        self.assertEqual(file_content[3].strip(), func_name)
        self.assertEqual(int(file_content[4].strip()), line_number)
        self.assertEqual(file_content[5].strip(), message)

        #Clean-up
        os.system("rm -rf %s"%file_name)

class logger(Structure):
    _fields_ = [('log_level' ,c_int),
                ('log_dir'   ,c_char_p),
                ('file_name' ,c_char_p)]

if __name__ == '__main__':
    unittest.main()
    #unittest.main(testRunner=HtmlTestRunner.HTMLTestRunner(output=RESULT_PATH))
