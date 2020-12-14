#!/usr/bin/env python

"""
"  @file      nibot_sensor_data.py
"  @brief     Program to acquire and print the sensor data from NiBot
"  @details   Connect to Nibot and print the sensor data in realtime
"  @author    Nitin Mohan
"  @date      December 11, 2020
"  @copyright 2020
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
import argparse
import json

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#
from lib_py.rsda_sock_lib import RSDAProc

#---------------------------------------------------#
#                   Global Varibles                 #
#---------------------------------------------------#

#--------------------------------------------------------------------------#
#                               Start of Program                           #
#--------------------------------------------------------------------------#
def handle_rsda_message(address, msg):
    
    """ 
    "  @brief            Process the recieved message
    "  param[in] address Address of Sender
    "  param[in] msg     Message that was sent
    """

    print (json.loads(msg))


if __name__ == '__main__':

    #-- Parse Arguments --#
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--ip_address', required=False, help="Enter IP Address for Remote Server")
    parser.add_argument('-m', '--mode', required=False, choices=["continous", "single"], default="continous", help="Enter mode of operation")
    parser.add_argument('-e', '--exit', required=False, action="store_true", help ="Shutdown the RSDA Process")
    args = parser.parse_args()

    if args.mode == "continous": continous = True
    else: continous = False

    #-- Create RSDA Object --#
    rsda = RSDAProc(server_ip=args.ip_address)

    if args.exit:
        rsda.stop_proc()
    else:
        #-- Listen on Socket --#
        rsda.listener.listen(handle_rsda_message, continous)
