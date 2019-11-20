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
        ScpClient nirobot_scp;
        string bin_path = "/home/nmohan/github/NiRobot/bin";
        string rsxa = "/home/nmohan/github/NiRobot/config/RSXA.json";
        string camera_horizontal_ct = "CAM_HRZN_MTR";
        string camera_vertical_ct = "CAM_VERT_MTR";
        string camera_control = "MTDR";

        public NiRobot()
        {
            InitializeComponent();
        }

        public class RSXA_Settings
        {
            public IDictionary<string, bool> rsxa_settings = new Dictionary<string, bool>();
            private string tmp_dir = @"C:\Windows\Temp\RSXA.json";

            /* Constructor */
            public RSXA_Settings(ScpClient nirobot_scp, string rsxa)
            {
                //Input     : NiRobot SCP Oboject and RSXA settings file
                //Output    : N/A - Constructor
                //Function  : Download the RSXA File from robot and read the settings

                /* Download the file from the robot over via SCP */
                using (Stream tmp_file = File.Create(tmp_dir))
                {
                    nirobot_scp.Download(rsxa, tmp_file);
                }

                /* Parse the contents of the file */
                using (StreamReader rsxa_stream = new StreamReader(tmp_dir))
                {
                    dynamic rsxa_obj = JsonConvert.DeserializeObject(rsxa_stream.ReadToEnd());

                    foreach (var hw in rsxa_obj.hw)
                    {
                        rsxa_settings.Add(new KeyValuePair<string, bool>((string)hw.hw_name, (bool)hw.hw_sim_mode));
                    }
                }
            }
        }

        private void NiRobot_Load(object sender, EventArgs e)
        {
            /* Set Intial state of all elements when the page loads */
            ip_disconnect_button.Enabled = false;
        }

        private void ip_connect_button_Click(object sender, EventArgs e)
        {
            //Input     : N/A - Button Click
            //Output    : N/A - Button Click
            //Function  : Create SSH and SCP Connection with robot

            /* Initialize Varibles */
            string ip_address = ip_textbox.Text;
            const int    ip_port    = 22;
            const string username   = "nmohan";
            const string password   = "nibot";

            /*Initialize SSH and SCP Objects */
            nirobot_ssh = new SshClient(ip_address, ip_port, username, password);
            nirobot_scp = new ScpClient(ip_address, ip_port, username, password);

            /* Try to Connect */
            try
            {
                nirobot_ssh.Connect();
                nirobot_scp.Connect();
            }
            catch 
            {
                MessageBox.Show("Unable to connect! Please check IP Address!");
                
            }

            /* If we're connected to the robot we proceed */
            if (nirobot_ssh.IsConnected && nirobot_scp.IsConnected)
            {
                /* Set Button States (Connect/Disconnect) */
                ip_connect_button.Enabled = false;
                ip_disconnect_button.Enabled = true;

                /* Get File Settings */
                RSXA_Settings robot_settings = new RSXA_Settings(nirobot_scp, rsxa);

                /* Populate Hardware List box */
                foreach (var hw_name in robot_settings.rsxa_settings.Keys)
                {
                    hw_list.Items.Add(hw_name);
                }
            }
                
           }

        private void left_button_Click(object sender, EventArgs e)
        {
            //Input     : N/A - Button Click
            //Output    : N/A - Button Click
            //Function  : Move the robot to the left

            int angle = 20;
            string robot_command = $"{bin_path}{camera_control} --motor {camera_horizontal_ct} --angle {angle}";
            SshCommand command = nirobot_ssh.RunCommand(robot_command);
            Console.WriteLine(command.Result);
        }

        private void ip_disconnect_button_Click(object sender, EventArgs e)
        {
            //Input     : N/A - Button Click
            //Output    : N/A - Button Click
            //Function  : Disconnect SSH and SCP connection

            nirobot_ssh.Disconnect();
            nirobot_scp.Disconnect();
            ip_connect_button.Enabled = true;
            ip_disconnect_button.Enabled = false;
        }
    }
}
