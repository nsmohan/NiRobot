#!/usr/bin/env python

"""
"  @file      move_motors.py
"  @brief     Script to move robot motors
"  @details   Control the robot Camera and Drive motors via command
              line interface
"  @author    Nitin Mohan
"  @date      April 2, 2020
"  @copyright 2020 - NM Technologies
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
import socket
import struct
import argparse
import json

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#
from lib_py import getip

#---------------------------------------------------#
#                   Constants                       #
#---------------------------------------------------#
""" @var RSXA_FILE
    Path to RSXA.json file """
RSXA_FILE = "/etc/NiBot/RSXA.json"

RMCT = "RMCT"
MAX_BUFF_SIZE = 10240

CAM_DIRECTIONS = ["UP", "DOWN", "LEFT", "RIGHT"]
DRV_DIRECTIONS = ["FORWARD", "REVERSE", "STOP"]
CAMERA_MOTORS  = ["CAMERA", "CAM_HRZN_MTR", "CAM_VERT_MTR"]
DRIVE_MOTORS   = ["LEFT_DRV_MTR", "RIGHT_DRV_MTR"]
DIRECTIONS     = CAM_DIRECTIONS + DRV_DIRECTIONS
MOTORS         = CAMERA_MOTORS + DRIVE_MOTORS

class MotorConnnect(object):

    def __init__(self):
        self.ip = str(getip.get_local_ip())
        self.__rsxa_settings()
        self.multi_sock_tx = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
        self.multi_sock_rx = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
        self.__config_multicast()
    
    def __del__(self):
        self.multi_sock_tx.close()
        self.multi_sock_rx.close()
    
    def __rsxa_settings(self):

        """ 
        "  @brief  Read the Robot Settings file and get needed settings
        """

        # -- Read the File -- #
        with open(RSXA_FILE, "r") as rsxa_file:
            rsxa = json.load(rsxa_file)

        # -- Find the Settings Needed -- #
        rmct_proc = filter(lambda p: p["proc_name"] == RMCT, rsxa["procs"])[0]

        # -- Load the Settings into the object -- #
        self.rmct_server_ip = rmct_proc["server_ip"]
        self.rmct_server_port = rmct_proc["server_p"]
        self.rmct_client_ip = rmct_proc["client_ip"]
        self.rmct_client_port = rmct_proc["client_p"]

    def show_rsxa_settings(self):
        print ("server_ip=%s\nclient_ip=%s\nserver_port=%d\nclient_port=%d"%(self.rmct_server_ip, self.rmct_client_ip,
                                                                             self.rmct_server_port, self.rmct_client_port))
    def __config_multicast(self):
        """ 
        "  @brief  Configure Client/Server sockets for communication with RMCT
        """

        # -- Configure the Server ---#
        self.multi_sock_tx.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.multi_sock_tx.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_IF, socket.inet_aton(self.ip))
        self.multi_sock_tx.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, 2)
        membership_request = socket.inet_aton(self.rmct_server_ip) + socket.inet_aton(self.ip)
        self.multi_sock_tx.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, membership_request)

        # -- Configure the Client -- #
        self.multi_sock_rx.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        mreq = struct.pack("4sl", socket.inet_aton(self.rmct_client_ip), socket.INADDR_ANY)
        self.multi_sock_rx.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)
        self.multi_sock_rx.bind((self.rmct_client_ip, self.rmct_client_port))

    def tx_message(self, message):
        """ 
        "  @brief                Send Message to RMCT over the socket
        "  @param[in]            message -> Message to send
        """

        print ("Sending request to NiBot ..... {}".format(message))
        self.multi_sock_tx.sendto(message,(self.rmct_server_ip, self.rmct_server_port))

    def rx_message(self):
        """ 
        "  @brief          Recieve message from RMCT
        "  @return         Message recieved from the server
        """

        return self.sock_multi.recv(MAX_BUFF_SIZE)

def construct_tx_message(motor, direction="", angle=-1, speed=-1):
    """ 
    "  @brief              Validate the arguements provided
    "  param[in] motor     Name of motor to be moved
    "  param[in] direction Direction to move in
    "  param[in] angle     Manually override motor angle
    "  param[in] speed     Set the speed of the motor
    """

    tx_message = {}
    tx_message["motor"] = motor
    tx_message["direction"] = direction
    tx_message["angle"] = angle
    tx_message["speed"] = speed

    return json.dumps(tx_message)


def valid_tx_message(motor, direction="", angle=-1, speed=-1):
    """ 
    "  @brief              Validate the arguements provided
    "  param[in] motor     Name of motor to be moved
    "  param[in] direction Direction to move in
    "  param[in] angle     Manually override motor angle
    "  param[in] speed     Set the speed of the motor
    """

    if (motor == "CAMERA"):
        pass
        #assert direction in CAM_DIRECTIONS, "Direction should be provided if motor == camera"
    elif (motor in CAMERA_MOTORS):
        assert angle >= 0, "Angle should be provided when motor == (CAM_HRZN_MTR or CAM_VERT_MTR)"
    elif (motor in DRIVE_MOTORS):
        assert direction in DRV_DIRECTIONS, "Drive direction should be provided when motor == {} expected = {}".format(direction, DRV_DIRECTIONS)

if __name__ == '__main__':

    # -- Parse arguments -- #
    parser = argparse.ArgumentParser()
    parser.add_argument('-m', '--motor', required=True, choices=MOTORS, help = "NiBot Motor to be moved")
    parser.add_argument('-d', '--direction', required=False, default="", choices=DIRECTIONS, help ="Directon the motor needs to be moved")
    parser.add_argument('-a', '--angle', required=False, type=int, default= -1, help ="Manually set the camera motor angle")
    parser.add_argument('-s', '--speed', required=False, type=int, default= -1, help ="Manually set drive motor speed")
    args = parser.parse_args()

    valid_tx_message(args.motor, args.direction, args.angle, args.speed)
    tx_message = construct_tx_message(args.motor, args.direction, args.angle, args.speed)

    mc = MotorConnnect()
    mc.tx_message(tx_message)
    

