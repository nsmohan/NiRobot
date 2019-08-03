#!/usr/bin/env python

"""unittest_NMTlib.py: Test all NMT_stdlib functions

__author__             = "Nitin Mohan
__copyright__          = "Copy Right 2019. NM Technologies"
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
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

class NMT_log_Test(unittest.TestCase):

    def setUp(self):
        self.NMT_log = CDLL(OBJ_PATH + "libNMT_log.so")

    def test_NMT_log_init(self):
        #Description - Test the log_init function and insure
        #              the values and correct in the log settings structure

        #Initialize Variables
        test_file = DAT_PATH + "tst_log_settings.json"

        #Read Settings file in Python
        f = open(test_file, "r")
        file_content = f.read()
        f.close()

        #Get Expected Values
        log_settings_exp  = json.loads(file_content)
        log_level_exp     = log_settings_exp["log_level"]
        log_dir_exp       = log_settings_exp["log_dir"]
        log_fname_exp     = __file__.split("/")[-1]
        log_verbosity_exp = True

        #Invoke Function
        self.NMT_log.NMT_log_init_m(__file__, test_file, True)
        log_settings_struct = logger.in_dll(self.NMT_log, 'log_settings')

        #Compare Actual vs Expected
        self.assertEqual(log_settings_struct.log_level, log_level_exp)
        self.assertEqual(log_settings_struct.log_dir, log_dir_exp)
        self.assertEqual(log_settings_struct.out_file_name, log_fname_exp)
        self.assertEqual(log_settings_struct.log_verbosity, log_verbosity_exp)

class logger(Structure):
    _fields_ = [('log_level'     ,c_char_p),
                ('log_dir'       ,c_char_p),
                ('out_file_name' ,c_char_p),
                ('log_verbosity' ,c_bool)]

if __name__ == '__main__':
    unittest.main()
    #unittest.main(testRunner=HtmlTestRunner.HTMLTestRunner(output=RESULT_PATH))
