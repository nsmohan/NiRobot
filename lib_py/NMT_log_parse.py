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
        log_op = []
        log_file = os.path.join(self.log_dir, self.log_name)

        # ---Read the Log File --#
        f = open(log_file, "r")
        log_data = f.read()
        f.close()

        # -- Parse the Log File -- #
        for line in log_data.splitlines():
            items = line.split("->")
            item = items[4].split(":")
            log_line = {}

            log_line["date"] = datetime.strptime(items[0], "%Y-%m-%d %H:%M:%S") 
            log_line["log_level"] = items[1]
            log_line["proc"] = items[2]
            log_line["method"] = items[3]
            log_line["line"] = item[0]
            log_line["message"] = item[1].strip()
            log_op.append(log_line)

        # -- Exit the Function -- #
        return log_op
