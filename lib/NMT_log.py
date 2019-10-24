#!/usr/bin/env python

"""NMT_log.py:         Python library for NMT_log.c

__author__             = "Nitin Mohan
__copyright__          = "Copy Right 2019. NM Technologies"
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
from ctypes import *

#Logger structure defintion for python use
class logger(Structure):
    _fields_ = [('log_level' ,c_int),
                ('log_dir'   ,c_char_p),
                ('file_name' ,c_char_p)]
