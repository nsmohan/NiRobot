#!/usr/bin/env python

"""
"  @file      unittest_NMTlib.py
"  @brief     Unittests for NMT_stdlib and NMT_log
"  @details   Test NMT_stdlib and NMT_log Interfaces
"  @author    Nitin Mohan
"  @date      August 3, 2019
"  @copyright 2020 - NM Technologies
"
"  @History DD/MM/YY Author Description
"           14/10/20 NITM   Add Tests for SwapBytes
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
from datetime import datetime
import numpy as np

#---------------------------------------------------#
#                   Constants                       #
#---------------------------------------------------#

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#
from lib_py import NMT_stdlib_py
from lib_py.NMT_stdlib_py import NMT_stdlib
from lib_py.NMT_stdlib_py import NMT_result
from lib_py.NMT_log import logger

class NMT_stdlib_test(unittest.TestCase):
    
    def setUp(self):
        pass

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

            no_of_param_c  = NMT_stdlib.NMT_stdlib_count(sp[0], sp[1])
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

            NMT_stdlib.NMT_stdlib_split(sp[0], sp[1], byref(item_array), byref(no_of_items))
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
        file_size = NMT_stdlib.NMT_stdlib_get_file_size(file_name)
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
        result =NMT_stdlib.NMT_stdlib_read_file(file_name, byref(file_content))
        for i in range(0, len(test_string)):
            c_string += file_content[i]

        self.assertEqual(result, NMT_result.OK)
        self.assertEqual(c_string, test_string)

        #Clean-up
        os.system("rm -rf %s"%file_name)

    def test_NMT_stdlib_read_file_BW(self):

        #Description - Pass incorrect file_name and ensure 
        #              read_file returns NOK

        #Initialize Variables
        file_name    = "/tmp/NMT_read_file_unittest.test"
        file_content = POINTER(c_char)()

        #Test Execution
        result = NMT_stdlib.NMT_stdlib_read_file(file_name, byref(file_content))

        self.assertEqual(result, NMT_result.NOK)

    def test_NMT_stdlib_write_file(self):
        #Description - Write to a file and verify the contents
        
        #Initialize Varibles
        file_name   = "/tmp/NMT_write_file_unittest.test"
        write_string = "Testing the NMT_stdlib_write_function"

        #Invoke the function
        NMT_stdlib.NMT_stdlib_write_file(file_name, write_string)

        #Get Actual Results
        f = open(file_name, "r")
        read_string = f.read().rstrip()
        f.close()

        #Verify Results
        self.assertEqual(read_string, write_string)

        #Clean-up
        os.system("rm -rf %s"%file_name)

    def test_NMT_stdlib_SwapBytes(self):

        #Description - Ensure SwapBytes is working

        #-- Initialize Varibles --#
        BytesToSwap = 0x1234
        SwapBytes_expected = 0x3412

        #-- Invoke Function --#
        SwappedBytes = np.int16(NMT_stdlib.NMT_stdlib_swapBytes(BytesToSwap))

        #-- Verify Result --#
        self.assertEqual(SwapBytes_expected, SwappedBytes)

    
    def test_NMT_timer_interrupt(self):

        #Description - Test NMT_timer_interrupt function
        
        cb = CFUNCTYPE(None) 
        cb_func = cb(dummy_function)

        print(cb, cb_func)

        func = NMT_stdlib.NMT_stdlib_timer_interrupt
        func(cb_func, 1000, False)


class NMT_log_Test(unittest.TestCase):

    def setUp(self):
        self.NMT_log   = CDLL("Obj/libNMT_log.so")
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

    def test_NMT_log_write_verbose(self):
        #Description - Test the log_write function by passing different inputs
        #              and verifying that outputs are as expected

        #Init Inputs
        line_number = 10
        func_name   = "Test_Function"
        message     = "Test Message"
        #Test 1 - Verbosity = True ? log_level == DEBUG
        #Test 2 - Verbosity = True ? log_level == WARNING
        #Test 3 - Verbosity = True ? log_level == ERROR
        log_level   = ["DEBUG", "WARNING", "ERROR"]
        verbosity   = [True, False]

        #Init log_settings struct in python
        log_settings_struct = logger.in_dll(self.NMT_log, 'log_settings')

        for v in verbosity:
            for level in range(0, len(log_level)):

                std_obj = NMT_stdlib_py.stdout_redirect()
                self.NMT_log.NMT_log_init_m(__file__, self.log_dir, v)
                self.NMT_log.NMT_log_write_m(line_number, func_name, level, message)

                file_name = "%s/%s.log"%(self.log_dir, self.log_fname)
                f = open(file_name, "r")
                file_content = f.read().split("->")
                f.close()
                
                #Split Line and Message and add to file_content
                line_message = file_content[-1]
                file_content.remove(line_message)
                line_message = line_message.split(":")
                file_content.extend([line_message[0], line_message[1]])

                #Get captured output from stdout
                captured_output = std_obj.capture_output().split("->")

                #Get current time for comparison later
                current_time = datetime.now().replace(microsecond=0)

                #Compare Actual vs Exppected --File
                log_time = datetime.strptime(file_content[0].strip(), "%Y-%m-%d %H:%M:%S")
                self.assertEqual(log_time, current_time)
                self.assertEqual(file_content[1].strip(), log_level[level])
                self.assertEqual(file_content[2].strip(), self.log_fname)
                self.assertEqual(file_content[3].strip(), func_name)
                self.assertEqual(int(file_content[4].strip()), line_number)
                self.assertEqual(file_content[5].strip(), message)

                #Compare Actual vs Exppected --stdout
                if not v and level == 0:
                    self.assertEqual(captured_output, [''])
                else:
                    #Split Line and Message and add to captured_output
                    line_message = captured_output[-1]
                    captured_output.remove(line_message)
                    line_message = line_message.split(":")
                    captured_output.extend([line_message[0], line_message[1]])

                    #Filter Date/Time
                    date_time = captured_output[0].replace("\x08", "").strip()
                    log_time = datetime.strptime(date_time, "%Y-%m-%d %H:%M:%S")

                    self.assertEqual(log_time, current_time)
                    self.assertEqual(captured_output[1].strip(), log_level[level])
                    self.assertEqual(captured_output[1].strip(), log_level[level])
                    self.assertEqual(captured_output[2].strip(), self.log_fname)
                    self.assertEqual(captured_output[3].strip(), func_name)
                    self.assertEqual(int(captured_output[4].strip()), line_number)
                    self.assertEqual(captured_output[5].strip(), message)

                #Clean-up
                os.system("rm -rf %s"%file_name)
                std_obj.set_default()

    def tearDown(self):
        os.system("rm -rf /tmp/*.log")

def dummy_function():
    print "I am a dummy function"

if __name__ == '__main__':
    unittest.main()
