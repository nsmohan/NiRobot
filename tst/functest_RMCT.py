#!/usr/bin/env python

"""functest_RMCT.py:   Functional Tests for RMCT Process

__author__             = "Nitin Mohan
__copyright__          = "Copy Right 2019. NM Technologies"
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
import unittest

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#

#---------------------------------------------------#
#                   Constants                       #
#---------------------------------------------------#
RSXA_FILE = "/etc/NiBot/RSXA.json"



# ---- Start of Tests -----#
class MTDR_Test(unittest.TestCase):

    def setUp(self):
        pass
    def tearDown(self):
        pass

if __name__ == '__main__':
    #unittest.main()
    get_log_data()
