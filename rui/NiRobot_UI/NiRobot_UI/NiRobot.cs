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
        RSXA_Settings robot_settings;
        string bin_path = "/home/nmohan/github/NiRobot/bld/";
        string rsxa = "/home/nmohan/github/NiRobot/config/RSXA.json";
        string camera_horizontal_ct = "CAM_HRZN_MTR";
        string camera_vertical_ct = "CAM_VERT_MTR";
        string camera_control = "mtdr";

        /* Structs and Enums */
        enum direction {UP, DOWN, LEFT, RIGHT, CUSTOM};

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
            //Input     : N/A - Button Click
            //Output    : N/A - Button Click
            //Function  : Create SSH and SCP Connection with robot

            /* Initialize Varibles */
            string ip_address = ip_combo_box.Text;
            const int    ip_port    = 22;
            const string username   = "nmohan";
            const string password   = "nibot";

            /*Initialize SSH and SFTP Objects */
            nirobot_ssh = new SshClient(ip_address, ip_port, username, password);
            nirobot_sftp = new SftpClient(ip_address, ip_port, username, password);

            /* Try to Connect */
            try
            {
                nirobot_ssh.Connect();
                nirobot_sftp.Connect();
            }
            catch 
            {
                MessageBox.Show("Unable to connect! Please check IP Address!");
                
            }

            /* If we're connected to the robot we proceed */
            if (nirobot_ssh.IsConnected && nirobot_sftp.IsConnected)
            {
                /* Set Button States (Connect/Disconnect) */
                ip_connect_button.Enabled = false;
                ip_disconnect_button.Enabled = true;
                hwgbox.Enabled = true;
                hw_init.Enabled = true;

                /* Update ComboBox and App Settings */
                if (!ip_combo_box.Items.Contains(ip_address))
                {
                    ip_combo_box.Items.Add(ip_address);
                    Properties.Settings.Default.ip_address.Add(ip_address);
                    Properties.Settings.Default.Save();
                }
                
                /* Get File Settings */
                robot_settings = new RSXA_Settings(nirobot_sftp, rsxa);

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

            int degrees = 10;
            move_camera(camera_horizontal_ct, direction.LEFT, degrees);
        }

        private void right_button_Click(object sender, EventArgs e)
        {
            //Input     : N/A - Button Click
            //Output    : N/A - Button Click
            //Function  : Move the robot to the Right

            int degrees = 10;
            move_camera(camera_horizontal_ct, direction.RIGHT, degrees);
        }

        private void up_button_Click(object sender, EventArgs e)
        {
            //Input     : N/A - Button Click
            //Output    : N/A - Button Click
            //Function  : Move the robot to the UP

            int degrees = 10;
            move_camera(camera_vertical_ct, direction.UP, degrees);
        }

        private void home_button_Click(object sender, EventArgs e)
        {
            //Input     : N/A - Button Click
            //Output    : N/A - Button Click
            //Function  : Move the robot to the HOME Postion

            int degrees = 90;
            move_camera(camera_horizontal_ct, direction.CUSTOM, degrees);
            move_camera(camera_vertical_ct, direction.CUSTOM, degrees);
        }

        private void down_button_Click(object sender, EventArgs e)
        {
            //Input     : N/A - Button Click
            //Output    : N/A - Button Click
            //Function  : Move the robot to the DOWN

            int degrees = 10;
            move_camera(camera_vertical_ct, direction.DOWN, degrees);
        }

        private void move_camera(string motor, direction direc, int degrees)
        {
            //Input     : N/A - Button Click
            //Output    : N/A - Button Click
            //Function  : Move specified motor in specified direction

            /* Initialize Varibles */
            string robot_command;
            string mtdr = $"{bin_path}{camera_control} --motor {motor}";
            double angle;
            SshCommand result;

            
            if (direc == direction.CUSTOM)
            {
                angle = degrees;
            }
            else
            {
                /* Get Current Position */
                robot_command = $"{mtdr} -c";
                Console.WriteLine(robot_command);
                result = nirobot_ssh.RunCommand(robot_command);

                if (result.ExitStatus == 0)
                {
                    /* Convert String to a double */
                    angle = Convert.ToDouble(result.Result);

                    if (direc == direction.UP || direc == direction.LEFT)
                    {
                        angle = angle + degrees;
                    }
                    else if (direc == direction.DOWN || direc == direction.RIGHT)
                    {
                        angle = angle - degrees;
                    }
                }
                else
                {
                    MessageBox.Show("Unable to get current Postion");
                    return;
                }
            }

            /* Move the actual Motor*/
            angle = (angle > 180 ? 180 : (angle < 0 ? 0 : angle));
            robot_command = $"{mtdr} --angle {angle}";
            Console.WriteLine(robot_command);
            result = nirobot_ssh.RunCommand(robot_command);
            if (result.ExitStatus != 0)
                {
                    MessageBox.Show("Unable to Move Motor");
                }
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
            string hw_name;
            bool sim_mode;

            hw_name = hw_list.GetItemText(hw_list.SelectedItem).ToString();
            sim_mode = robot_settings.rsxa_settings[hw_name];
            set_hw_button_states(sim_mode);
        }

        private void set_hw_button_states(bool sim_mode)
        {
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
            robot_settings.Update_RSXA_Settings(nirobot_sftp, rsxa, hw_name);
        }

        private void hw_init_Click(object sender, EventArgs e)
        {
            string robot_command = $"{bin_path}{camera_control} -i";
            SshCommand result = nirobot_ssh.RunCommand(robot_command);

            if(result.ExitStatus != 0)
            {
                MessageBox.Show("Error! Unable to Intialize Driver");
            }
        }
    }

    public class RSXA_Settings
    {
        /* Global Varibles */
        public IDictionary<string, bool> rsxa_settings = new Dictionary<string, bool>();
        private dynamic rsxa_obj;
        private string tmp_dir = @"C:\Windows\Temp\RSXA.json";

        /* Constructor */
        public RSXA_Settings(SftpClient nirobot_sftp, string rsxa)
        {
            //Input     : NiRobot SCP Oboject and RSXA settings file
            //Output    : N/A - Constructor
            //Function  : Download the RSXA File from robot and read the settings

            /* Download the file from the robot over via SFTP */
            using (Stream tmp_file = File.Create(tmp_dir))
            {
                nirobot_sftp.DownloadFile(rsxa, tmp_file);
            }

            /* Parse the contents of the file */
            using (StreamReader rsxa_stream = new StreamReader(tmp_dir))
            {
                rsxa_obj = JsonConvert.DeserializeObject(rsxa_stream.ReadToEnd());

                foreach (var hw in rsxa_obj.hw)
                {
                    this.rsxa_settings.Add(new KeyValuePair<string, bool>((string)hw.hw_name, (bool)hw.hw_sim_mode));
                }
            }
        }

        public void Update_RSXA_Settings(SftpClient nirobot_sftp, string rsxa, string hw_name)
        {

            foreach (var hw in rsxa_obj.hw)
            {
                if (hw.hw_name == hw_name)
                {
                    hw.hw_sim_mode = this.rsxa_settings[hw_name];
                }
            }

            using (StreamWriter file = File.CreateText(tmp_dir))
            {
                JsonSerializer serializer = new JsonSerializer();
                serializer.Formatting = Formatting.Indented;
                serializer.Serialize(file, rsxa_obj);
            }

            /* Upload the file via SFTP to robot */
            using (Stream rsxa_local = File.OpenRead(tmp_dir))
            {
                nirobot_sftp.UploadFile(rsxa_local, rsxa);
            } 
        }
    }
}
