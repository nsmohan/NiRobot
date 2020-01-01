#!/usr/bin/env python

"""PCA9685.py:         Python library for PCA9685.c

__author__             = "Nitin Mohan
__copyright__          = "Copy Right 2019. NM Technologies"
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
from ctypes import *

#PCA9685 Settings Struct
class PCA9685_settings(Structure):
    _fields_ = [('fd'         ,c_int),
                ('freq'       ,c_float),
                ('duty_cycle' ,c_double),
                ('delay_time' ,c_double)]
