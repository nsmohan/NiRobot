#!/usr/bin/env python

"""NMT_stdlib_py.py: Library for standard python functions

__author__             = "Nitin Mohan
__copyright__          = "Copy Right 2019. NM Technologies"
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
import sys
import os
import select
from ctypes import *

#Create NMT_stdlib Python Object
NMT_stdlib = CDLL("Obj/libNMT_stdlib.so")

#NMT_result ENUM
class NMT_result():
    __slots__ = ('OK', 'NOK')
    OK = 0
    NOK = 1

    @staticmethod
    def get_result(status):
        if status:
            return "OK"
        else:
            return "NOK"

class stdout_redirect(object):

    #Description - Utility to redirect the stdout to a varible

    def __init__(self):
        sys.stdout.write(' \b')
        self.pipe_out, self.pipe_in = os.pipe()
        self.stdout = os.dup(1)               # save a copy of stdout
        os.dup2(self.pipe_in, 1)              # replace stdout with our write pipe

    def capture_output(self):
        out = ''
        r = True
        while bool(r):
                r, _, _ = select.select([self.pipe_out], [], [], 0)
                if self.pipe_out in r:
                    out += os.read(self.pipe_out, 1024)
        return out
    
    def set_default(self):
        os.dup2(self.stdout, 1)
