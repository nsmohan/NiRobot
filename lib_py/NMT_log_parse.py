#!/usr/bin/env python

"""
"  @file      NMT_log_parse.py
"  @brief     Script to Parse the NMT logs
"  @details   Program parses NMT logs into a dict object
"  @author    Nitin Mohan
"  @date      April 6, 2020
"  @copyright 2020 - NM Technologies
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
import os
import json
from datetime import datetime

#---------------------------------------------------#
#                   Constants                       #
#---------------------------------------------------#
RSXA_FILE = "/etc/NiBot/RSXA.json"


# --- Class Implementation ---#
class NMT_Log_Parse(object):
    def __init__(self, LOG_NAME):
        self.log_name = LOG_NAME
        self.log_dir = self.__get_log_dir()
        self.log_data = self.__get_log_data()

    def __get_log_dir(self):
        """ 
        "  @brief  Read the Robot Settings file and get needed settings
        """

        # -- Read the File -- #
        with open(RSXA_FILE, "r") as rsxa_file:
            rsxa = json.load(rsxa_file)
        return rsxa["log_dir"]

    def __get_log_data(self):
        """ 
        "  @brief  Parse the log data and place in dictionaty object
        """

        # -- Init Varibles -- # 
        rmct_log_op = []
        rmct_file = os.path.join(self.log_dir, self.log_name)

        # ---Read the Log File --#
        f = open(rmct_file, "r")
        rmct_log_data = f.read()
        f.close()

        # -- Parse the Log File -- #
        for line in rmct_log_data.splitlines():
            items = line.split("->")
            item = items[4].split(":")
            rmct_log = {}

            rmct_log["date"] = datetime.strptime(items[0], "%Y-%m-%d %H:%M:%S") 
            rmct_log["log_level"] = items[1]
            rmct_log["proc"] = items[2]
            rmct_log["method"] = items[3]
            rmct_log["line"] = item[0]
            rmct_log["message"] = item[1].strip()
            rmct_log_op.append(rmct_log)

        # -- Exit the Function -- #
        return rmct_log_op

nm = NMT_Log_Parse("RMCT.log")
print nm.log_data
