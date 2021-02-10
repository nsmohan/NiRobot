#!/usr/bin/env python

"""RSXA.py:             Python Library for RSXA.c

__author__             = "Nitin Mohan
__copyright__          = "Copy Right 2019. NM Technologies"
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
from ctypes import *

#---------------------------------------------------#
#                   Globals                         #
#---------------------------------------------------#
MAX_LEN_1 = 20
MAX_LEN_2 = 100

#Create RSXA Object
rsxa = CDLL("Obj/libRSXA.so")

#RSXA General Settings Struct
class RSXA_general_settings(Structure):
    _fields_ = [('cam_mtr_step_size'  , c_int),
                ('default_drive_speed', c_int),
                ('rsda_broadcast_freq', c_int)]
                

#RSXA Pins Struct
class RSXA_pins(Structure):
    _fields_ = [('pin_name', c_char * MAX_LEN_1),
                ('pin_no'  , c_int)]

#RSXA Settings Struct
class RSXA_hw(Structure):
    _fields_ = [('hw_name'     ,c_char * MAX_LEN_1),
                ('hw_sim_mode' ,c_bool),
                ('hw_interface',POINTER(RSXA_pins)),
                ('array_len_hw_int', c_int)]

# RSXA procs
class RSXA_procs(Structure):
    _fields_ = [('proc_name', c_char * MAX_LEN_1),
                ('server_ip', c_char * MAX_LEN_1 ),
                ('server_p',  c_int),
                ('client_ip' ,c_char * MAX_LEN_1),
                ('client_p', c_int)]
# RSXA Struct
class RSXA(Structure):
    _fields_ = [('log_dir', c_char * MAX_LEN_2),
                ('procs',POINTER(RSXA_procs)),
                ('hw'     , POINTER(RSXA_hw)),
                ('general_settings', RSXA_general_settings),
                ('array_len_procs', c_int),
                ('array_len_hw', c_int)]