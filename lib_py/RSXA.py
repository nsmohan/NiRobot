#!/usr/bin/env python

"""RSXA.py:             Python Library for RSXA.c

__author__             = "Nitin Mohan
__copyright__          = "Copy Right 2019. NM Technologies"
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
from ctypes import *

#RSXA Pins Struct
class RSXA_pins(Structure):
    _fields_ = [('pin_name', c_char * 20),
                ('pin_no'  , c_int)]

#RSXA Settings Struct
class RSXA_hw(Structure):
    _fields_ = [('hw_name'     ,c_char * 20),
                ('hw_sim_mode' ,c_bool),
                ('hw_interface',POINTER(RSXA_pins)),
                ('array_len_hw_int', c_int)]

# RSXA Struct
class RSXA(Structure):
    _fields_ = [('log_dir', c_char * 100),
                ('hw'     , POINTER(RSXA_hw)),
                ('array_len_hw', c_int)]
