#!/usr/bin/env python

"""unittest_NMTlib.py: Test all NMT_stdlib functions

__author__             = "Nitin Mohan
__copyright__          = "Copy Right 2019. NM Technologies"
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
import inspect
import unittest
import re
from ctypes import *

#---------------------------------------------------#
#                   Constants                       #
#---------------------------------------------------#
OBJ_PATH = "/home/nmohan/Documents/github/NiRobot/Obj/"

class NMT_stdlib_test(unittest.TestCase):
    
    def setUp(self):
        self.NMT_stdlib = CDLL(OBJ_PATH + "libNMT_stdlib.so")

    def test_NMT_stdlib_count(self):

        #Description - Pass various strings to function with token to count\
        #              and ensure the correct value is returned.

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

if __name__ == '__main__':
    unittest.main()
