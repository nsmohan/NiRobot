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
        public class RSXA_Settings
        {
            /* Global Varibles */
            public IDictionary<string, bool> rsxa_settings = new Dictionary<string, bool>();
            private dynamic rsxa_obj;
            private string tmp_dir = @"C:\Windows\Temp\RSXA.json";
            private string rsxa = "/home/nmohan/github/NiRobot/config/RSXA.json";

            /* Constructor */
            public RSXA_Settings(SftpClient nirobot_sftp)
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

            public void Update_RSXA_Settings(SftpClient nirobot_sftp, string hw_name)
            {
                //Input     : NiRobot_sftp, Location of RSXA file, hw that has changed
                //Output    : N/A - Void
                //Function  : Update the rsxa_obj with the new setting and upload file to robot

                /* Update rsxa_obj */
                foreach (var hw in rsxa_obj.hw)
                {
                    if (hw.hw_name == hw_name)
                    {
                        hw.hw_sim_mode = this.rsxa_settings[hw_name];
                    }
                }

                /* Save the updated JSON File */
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
}
