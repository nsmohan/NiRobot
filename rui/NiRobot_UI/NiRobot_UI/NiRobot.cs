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
        RSXA_Settings robot_settings;
        int default_movement = 10;


        /* Structs and Enums */
        enum direction {UP, DOWN, LEFT, RIGHT, HOME};

        public NiRobot()
        {
            InitializeComponent();
            InitIpComboBox();
        }

        private void InitIpComboBox()

        {
            //Input     : N/A
            //Output    : N/A
            //Function  : Read Application settings and update the IP ComboBox

            if (Properties.Settings.Default.ip_address != null)
            {
                foreach (string ip in Properties.Settings.Default.ip_address)
                {
                    ip_combo_box.Items.Add(ip);
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
            /* Initialize Varibles */
            string ip_address = ip_combo_box.Text;
            const int ip_port = 22;
            const string username = "nmohan";
            const string password = "nibot";

            /* Connnect to Robot */
            nibot_connect(username,
                          password,
                          ip_address,
                          ip_port);

            /* Set Button States (Connect/Disconnect) */
            ip_connect_button.Enabled = false;
            ip_disconnect_button.Enabled = true;
            hwgbox.Enabled = true;
            ccgbox.Enabled = true;
                
            /* Update ComboBox and App Settings */
            if (!ip_combo_box.Items.Contains(ip_address))
            {
                ip_combo_box.Items.Add(ip_address);
                Properties.Settings.Default.ip_address.Add(ip_address);
                Properties.Settings.Default.Save();
            }
                
            /* Get File Settings */
            robot_settings = new RSXA_Settings(nirobot_sftp);

            /* Populate Hardware List box */
            foreach (var hw_name in robot_settings.rsxa_settings.Keys)
            {
                hw_list.Items.Add(hw_name);
            }
            
                
           }

        private void left_button_Click(object sender, EventArgs e)
        {
            //Input     : N/A - Button Click
            //Output    : N/A - Button Click
            //Function  : Move the robot to the left

            Move_camera(direction.LEFT);
        }

        private void right_button_Click(object sender, EventArgs e)
        {
            //Input     : N/A - Button Click
            //Output    : N/A - Button Click
            //Function  : Move the robot to the Right
            Move_camera(direction.RIGHT);
        }

        private void up_button_Click(object sender, EventArgs e)
        {
            //Input     : N/A - Button Click
            //Output    : N/A - Button Click
            //Function  : Move the robot to the UP

            Move_camera(direction.UP);
        }

        private void down_button_Click(object sender, EventArgs e)
        {
            //Input     : N/A - Button Click
            //Output    : N/A - Button Click
            //Function  : Move the robot to the DOWN

            Move_camera(direction.DOWN);
        }

        private void home_button_Click(object sender, EventArgs e)
        {
            //Input     : N/A - Button Click
            //Output    : N/A - Button Click
            //Function  : Move the robot to the HOME Postion
            Move_camera(direction.HOME);
        }

        private void ip_disconnect_button_Click(object sender, EventArgs e)
        {
            //Input     : N/A - Button Click
            //Output    : N/A - Button Click
            //Function  : Disconnect SSH and SFTP connection

            nirobot_ssh.Disconnect();
            nirobot_sftp.Disconnect();
            ip_connect_button.Enabled = true;
            ip_disconnect_button.Enabled = false;
            hw_list.Items.Clear();
            hw_sim_mode.BackColor = default(Color);
            hw_real_mode.BackColor = default(Color);
            hwgbox.Enabled = false;
            hw_sim_mode.Enabled = false;
            hw_real_mode.Enabled = false;
            hw_init.Enabled = false;
        }

        private void hw_list_SelectedIndexChanged(object sender, EventArgs e)
        {
            //Input     : N/A - Button Click
            //Output    : N/A - Button Click
            //Function  : Check Settings for the hw_name selected

            string hw_name;
            bool sim_mode;

            hw_name = hw_list.GetItemText(hw_list.SelectedItem).ToString();

            if (!string.IsNullOrEmpty(hw_name))
            {
                sim_mode = robot_settings.rsxa_settings[hw_name];
                set_hw_button_states(sim_mode);
            }

        }

        private void set_hw_button_states(bool sim_mode)
        {
            //Input     : N/A - Button Click
            //Output    : N/A - Button Click
            //Function  : Set Button states for (Sim and Real mode)

            if (sim_mode)
            {
                /*Sim Button States */
                hw_sim_mode.Enabled = false;
                hw_sim_mode.BackColor = Color.FromArgb(192, 255, 192);

                /* Real Button States */
                hw_real_mode.Enabled = true;
                hw_real_mode.BackColor = default(Color);
            }
            else
            {
                /*Sim Button States */
                hw_sim_mode.Enabled = true;
                hw_sim_mode.BackColor = default(Color);

                /* Real Button States */
                hw_real_mode.Enabled = false;
                hw_real_mode.BackColor = Color.FromArgb(192, 255, 192);
            }
        }

        private void hw_sim_mode_Click(object sender, EventArgs e)
        {
            bool sim_mode = true;
            update_hw_mode(sim_mode);
            set_hw_button_states(sim_mode);
        }

        private void hw_real_mode_Click(object sender, EventArgs e)
        {
            bool sim_mode = false;
            update_hw_mode(sim_mode);
            set_hw_button_states(sim_mode);
        }

        private void update_hw_mode(bool sim_mode)
        {
            string hw_name = hw_list.GetItemText(hw_list.SelectedItem);
            robot_settings.rsxa_settings[hw_name] = sim_mode;
            robot_settings.Update_RSXA_Settings(nirobot_sftp, hw_name);
        }

        private void hw_init_Click(object sender, EventArgs e)
        {
        }
    }
}
