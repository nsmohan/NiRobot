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
        private string camera_horizontal_ct = "CAM_HRZN_MTR";
        private string camera_vertical_ct = "CAM_VERT_MTR";
        private string camera_control = "mtdr";

        enum motor_operation { INIT_PCA9685, 
                               GET_PCA9685_STATUS, 
                               GET_CURRENT_POSITION, 
                               MOVE_MOTOR }

        enum hw_status { INITIALIZED, NOT_INITIALIZED }


        private string command(motor_operation operation, string motor = "", string angle = "")
        {
            //Input     : Operation, (Optional: motor_name and angle)
            //Output    : Command to execute input operation
            //Function  : Generate command string based on operation

            string mtdr = $"{bin_path}{camera_control}";

            switch (operation)
            {
                case motor_operation.INIT_PCA9685:
                    return $"{mtdr} -i";
                case motor_operation.GET_PCA9685_STATUS:
                    return $"{mtdr} -s";
                case motor_operation.GET_CURRENT_POSITION:
                    return $"{mtdr} --motor {motor} -c";
                case motor_operation.MOVE_MOTOR:
                    return $"{mtdr} --motor {motor} --angle {angle}";
                default:
                    return "";
            }
        }

        private void Move_camera(string motor, direction direc, int degrees)
        {
            //Input     : N/A - Button Click
            //Output    : N/A - Button Click
            //Function  : Move specified motor in specified direction

            double angle = 0;
            string output; 


            if (direc == direction.CUSTOM)
            {
                angle = degrees;
            }
            else
            {
                /* Get Current Position */
                output = send_command(command(motor_operation.GET_CURRENT_POSITION, motor));

                /* Convert String to a double */
                angle = Convert.ToDouble(output);

                if (direc == direction.UP || direc == direction.LEFT)
                {
                    angle = angle + degrees;
                }
                else if (direc == direction.DOWN || direc == direction.RIGHT)
                {
                    angle = angle - degrees;
                }
                else
                {
                    MessageBox.Show("Unable to get current Postion");
                    return;
                }
            }

            /* Move the actual Motor*/
            angle = (angle > 180 ? 180 : (angle < 0 ? 0 : angle));
            nirobot_ssh.RunCommand(command(motor_operation.MOVE_MOTOR,
                                            motor,
                                            angle.ToString()));
        }

        private hw_status get_hw_status(string hw_name)
        {
            //Input     : Hardware Name
            //Output    : Status (Initialized/Not Initialzed)
            //Function  : Query the robot for the status on specified hardware

            string output;
            
            output = send_command(command(motor_operation.GET_PCA9685_STATUS));

            if (Convert.ToBoolean(output))
            {
                return hw_status.INITIALIZED;
            }
            else
            {
                return hw_status.NOT_INITIALIZED;
            }
        }
    }
}
