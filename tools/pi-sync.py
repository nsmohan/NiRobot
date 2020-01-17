#!/usr/bin/env python

"""pi-sync.py:         Program to sync chagned code to the desired host

__author__             = "Nitin Mohan
__copyright__          = "Copy Right 2019. NM Technologies"
"""
#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
import argparse
import subprocess
import os

#---------------------------------------------------#
#                   Constants                       #
#---------------------------------------------------#
DEST_PATH = "/home/nmohan/github/NiRobot"

def file_list():
    
    # -- Get List of Modified Files -- #
    mod_file_list  = filter(None, subprocess.check_output(["git", 
                                                          "diff", 
                                                          "--name-only"]).split("\n"))

    # -- Get List of New Files -- #
    new_file_list = filter(None, subprocess.check_output(["git", 
                                                          "ls-files", 
                                                          "--others", 
                                                          "--exclude-standard"]).split("\n"))

    # -- Combine list of new files + modified files -- #
    return mod_file_list + new_file_list


def sync(file_list, hostname):
    # -- SCP the files to the host -- #
    for f in file_list:
        os.system("scp %s %s:%s/%s"%(f, hostname, DEST_PATH, f))

if __name__ == '__main__':
    #Parse arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('-s', '--server', required = True,  help = "-s/--server Enter the hostname")
    args = parser.parse_args()

    # -- Get list of changed files -- #
    file_list = file_list()

    # -- Sync files -- #
    sync(file_list, args.server)

