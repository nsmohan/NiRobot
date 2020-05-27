#!/bin/bash

#install.sh:     Script to Install NiBot Software
#__author__      = Nitin Mohan
#__copyright__   = Copy Right 2018. NM Technologies


#---------------------------------------------------------------------#
#                            Global Variables                         #
#---------------------------------------------------------------------#
LOG_DIR=/var/log/NiBot
CNF_DIR=/etc/NiBot
SRC_CNF_DIR=config

#---------------------------------------------------------------------#
#                            Start of Program                         #
#---------------------------------------------------------------------#

# -----------------Check if script is being run as root --------------#
if ! id |grep -q root; then
    echo "Must be root user to install application"
    exit
fi


# --- Create Required Directories ----#
if [ ! -d $LOG_DIR ]; then mkdir $LOG_DIR; fi
if [ ! -d $CNF_DIR ]; then mkdir $CNF_DIR; fi
 
# -- Copy Needed Files ---------#
cp $SRC_CNF_DIR/RSXA.json $CNF_DIR

# -----Set Appropriate Permissions --#
chmod 777 $CNF_DIR
chmod -R 777 $LOG_DIR
 
echo "**************** NIBOT Configuration Complete! *************************"
