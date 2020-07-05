#!/usr/bin/env python

""""  
"  @file      NMT_transport.py
"  @brief     Library for creating SCP/SSH Connections
"  @details   Wrapper library for Paramiko
"  @author    Nitin Mohan
"  @date      May 6, 2020
"  @copyright 2020 - NM Technologies

__author__             = "Nitin Mohan
__copyright__          = "Copy Right 2019. NM Technologies"
"""
#---------------------------------------------------#
#                   System Imports                  #
#---------------------------------------------------#
import paramiko
import scp

#---------------------------------------------------#
#                   Constants                       #
#---------------------------------------------------#
PORT = 22

#------------------Start of Program ----------------#
class NMT_transport(object):

    def __init__(self, hostname, username="", password=""):

        """ 
        "  @brief              Constructor
        "  param[in] hostname  Name of Host
        "  param[in] username  Username
        "  param[in] password  Password
        """

        # -- Initialize Variables -- #
        self.ssh = None
        self.scp = None
        self.hostname = hostname
        self.username = username
        self.password = password

        #-- Initialize Class --#
        self.__sshConnect()
        self.__scpConnect()

    #---------------------------------------------------#
    #                   Private Methods                 #
    #---------------------------------------------------#
    def __del__(self):

        """ 
        "  @brief Destructor
        """

        if self.ssh:
            self.ssh.close()
        if self.scp:
            self.scp.close()

    def __sshConnect(self):

        """ 
        "  @brief Create SSH Connection
        """

        try:
            self.ssh = paramiko.SSHClient()
            self.ssh.load_system_host_keys()
            self.ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())

            if self.username and self.password:
                self.ssh.connect(self.hostname, port=PORT, username=self.username, password=self.password)
            else:
                self.ssh.connect(self.hostname, port=PORT)
        except paramiko.AuthenticationException:
            raise Exception("Authentication Failed!")
        except paramiko.SSHException as sshException:
            raise Exception("SSH Connection Failed!")
        except paramiko.BadHostKeyException as badHostKeyException:
            raise Exception("Bad Host Key!")


    def __scpConnect(self):

        """ 
        "  @brief Create SCP Connection
        """

        try:
            self.scp = scp.SCPClient(self.ssh.get_transport())
        except scp.SCPException as e:
            raise Exception("Unable to Create SCP Connection!")

    #---------------------------------------------------#
    #                   Public Methods                  #
    #---------------------------------------------------#
    def send_command(self, command):

        """ 
        "  @brief Send command to host
        "  param[in] command -> Command to be sent
        """

        try:
            stdin, stdout, stderr = self.ssh.exec_command(command)
        except scp.SSHException as sshException:
            raise Exception("Failed to Send Command to Host!")
        
        return stdout.read(), stderr.read()

    def send_file(self, source_file, remote_path):

        """ 
        "  @brief Send file to Host
        """
        try:
            self.scp.put(source_file, recursive=True, remote_path=remote_path) 
        except scp.SCPException:
            raise Exception("Failed to send file to Host!")

    def get_file(self, remote_path, local_path):

        """ 
        "  @brief Get File from Host
        """

        try:
            self.scp.get(remote_path=remote_path, recursive=True, local_path=local_path) 
        except scp.SCPException:
            print("Reading file")
            raise Exception("Failed to Get file from Host!")
