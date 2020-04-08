#!/usr/bin/env python

"""
"  @file      nibot_mtr_ctrl.py
"  @brief     Script to send commands to RMCT
"  @details   Communicate with RMCT via the command line interface
"  @author    Nitin Mohan
"  @date      April 7, 2020
"  @copyright 2020 - NM Technologies
"""

#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
import argparse
import json

#---------------------------------------------------#
#                   Local Imports                   #
#---------------------------------------------------#
from lib_py.NMT_stdlib_py import NMT_result
from lib_py.rmct_sock_lib import RMCTSockConnect

#---------------------------------------------------#
#                   Constants                       #
#---------------------------------------------------#
CAM_DIRECTIONS = ["UP", "DOWN", "LEFT", "RIGHT"]
DRV_DIRECTIONS = ["FORWARD", "REVERSE", "STOP"]
CAMERA_MOTORS  = ["CAMERA", "CAM_HRZN_MTR", "CAM_VERT_MTR"]
DRIVE_MOTORS   = ["LEFT_DRV_MTR", "RIGHT_DRV_MTR"]
DIRECTIONS     = CAM_DIRECTIONS + DRV_DIRECTIONS
MOTORS         = CAMERA_MOTORS + DRIVE_MOTORS

#---------------------------------------------------#
#                   Start of Program                #
#---------------------------------------------------#

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
    parser.add_argument('-m', '--motor', required=False, choices=MOTORS, help = "NiBot Motor to be moved")
    parser.add_argument('-d', '--direction', required=False, default="", choices=DIRECTIONS, help ="Directon the motor needs to be moved")
    parser.add_argument('-a', '--angle', required=False, type=int, default= -1, help ="Manually set the camera motor angle")
    parser.add_argument('-s', '--speed', required=False, type=int, default= -1, help ="Manually set drive motor speed")
    parser.add_argument('-e', '--exit', required=False, action="store_true", help ="Shutdown the RMCT Process")
    args = parser.parse_args()

    rmct = RMCTSockConnect()
    if (args.exit):
        tx_message = json.dumps({"type": "proc_action", "action": "exit"})
    else:
        valid_tx_message(args.motor, args.direction, args.angle, args.speed)
        tx_message = rmct.construct_tx_message(args.motor, args.direction, args.angle, args.speed)

    # -- Send the Command -- #
    rmct.tx_message(tx_message)

    print("NiBOT Response=%s"%(NMT_result.get_result(rmct.rx_message()["result"])))

    

