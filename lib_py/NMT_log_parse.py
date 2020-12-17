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

# --- Implementation ---#
def parse_log_data(log_dir, log_name):
    """ 
    "  @brief  Parse the log data and place in dictionaty object
    """

    # -- Init Varibles -- # 
    log_op = []
    log_file = os.path.join(log_dir, log_name)

    # ---Read the Log File --#
    f = open(log_file, "r")
    log_data = f.read()
    f.close()

    # -- Parse the Log File -- #
    for line in log_data.splitlines():
        items = line.split("->")
        item = items[4].split(":")
        log_line = {}

        log_line["date"] = datetime.strptime(items[0], "%m-%d-%Y %H:%M:%S.%f") 
        log_line["log_level"] = items[1]
        log_line["proc"] = items[2]
        log_line["method"] = items[3]
        log_line["line"] = item[0]
        log_line["message"] = item[1].strip()
        log_op.append(log_line)

    # -- Exit the Function -- #
    return log_op
