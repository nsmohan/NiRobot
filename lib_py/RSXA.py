#!/usr/bin/env python

"""RSXA.py:             Python Library for RSXA.c

__author__             = "Nitin Mohan
__copyright__          = "Copy Right 2019. NM Technologies"
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
from ctypes import *

#RSXA Settings Struct
class RSXA_hw(Structure):
    _fields_ = [('hw_name'     ,POINTER(c_char_p)),
                ('hw_sim_mode' ,POINTER(c_bool)),
                ('array_len'   ,c_int)]
