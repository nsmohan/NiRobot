using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Renci.SshNet;
using Newtonsoft.Json;

namespace NiRobot_UI
{
    public partial class NiRobot : Form
    {
        /* Global Varibles */
        SshClient nirobot_ssh;
        SftpClient nirobot_sftp;

        private void nibot_connect(string un, string pw, string ip, int port)                   
        {
            //Input     : N/A - Button Click
            //Output    : N/A - Button Click
            //Function  : Create SSH and SCP Connection with robot

            /*Initialize SSH and SFTP Objects */
            nirobot_ssh = new SshClient(ip, port, un, pw);
            nirobot_sftp = new SftpClient(ip, port, un, pw);

            /* Try to Connect */
            try
            {
                nirobot_ssh.Connect();
                nirobot_sftp.Connect();
            }
            catch
            {
                MessageBox.Show("Unable to connect! Please check IP Address!");
                throw new System.InvalidOperationException("Unable Connect");

            }
        }

        private string send_command(string command)
        {
            SshCommand result;
            Console.WriteLine($"Sending Command: {command}");

            try
            {
                result = nirobot_ssh.RunCommand(command);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
                throw;
            }
            

            if (result.ExitStatus != 0)
            {
                throw new System.InvalidOperationException($"Command Failed: {command}");
            }
            else
            {
                return result.Result;
            }
        }

    }
}