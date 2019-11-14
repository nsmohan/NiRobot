using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Renci.SshNet;

namespace NiRobot_UI
{

    
    public partial class NiRobot : Form
    {
        /* Global Varibles */
        SshClient nirobot;

        public NiRobot()
        {
            InitializeComponent();
        }

        private void NiRobot_Load(object sender, EventArgs e)
        {
            ip_disconnect_button.Enabled = false;
        }

        private void ip_connect_button_Click(object sender, EventArgs e)
        {
            string       ip_address = ip_textbox.Text;
            const int    ip_port    = 22;
            const string username   = "nmohan";
            const string password   = "nibot";

            nirobot = new SshClient(ip_address, 22, username, password);

            try
            {
                nirobot.Connect();
                ip_connect_button.Enabled = false;
                ip_disconnect_button.Enabled = true;
            }
            catch 
            {
                MessageBox.Show("Unable to connect! Please check IP Address!");
                
            }
                
           }

        private void left_button_Click(object sender, EventArgs e)
        {
            SshCommand command = nirobot.RunCommand("ls");
            Console.WriteLine(command.Result);

        }

        private void ip_disconnect_button_Click(object sender, EventArgs e)
        {
            nirobot.Disconnect();
            ip_connect_button.Enabled = true;
            ip_disconnect_button.Enabled = false;
        }
    }
}
