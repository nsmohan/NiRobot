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
        public string bin_path = "/home/nmohan/github/NiRobot/bld/";
        private string camera_control = "camera_control";

        private void Move_camera(direction direc)
        {
            //Input     : N/A - Button Click
            //Output    : N/A - Button Click
            //Function  : Move specified motor in specified direction
            string dir_string = direc.ToString();
            string command = $"{bin_path}{camera_control} --direction {dir_string}";
            nirobot_ssh.RunCommand(command);
        }
    }
}
