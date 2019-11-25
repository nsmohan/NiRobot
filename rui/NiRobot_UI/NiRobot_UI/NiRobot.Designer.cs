namespace NiRobot_UI
{
    partial class NiRobot
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.ip_connect_button = new System.Windows.Forms.Button();
            this.ip_label = new System.Windows.Forms.Label();
            this.ip_disconnect_button = new System.Windows.Forms.Button();
            this.left_button = new System.Windows.Forms.Button();
            this.backgroundWorker1 = new System.ComponentModel.BackgroundWorker();
            this.hwgbox = new System.Windows.Forms.GroupBox();
            this.hw_init = new System.Windows.Forms.Button();
            this.hw_real_mode = new System.Windows.Forms.Button();
            this.hw_sim_mode = new System.Windows.Forms.Button();
            this.hw_status_label = new System.Windows.Forms.Label();
            this.hw_mode_label = new System.Windows.Forms.Label();
            this.hw_list = new System.Windows.Forms.ListBox();
            this.ip_combo_box = new System.Windows.Forms.ComboBox();
            this.ccgbox = new System.Windows.Forms.GroupBox();
            this.home_button = new System.Windows.Forms.Button();
            this.down_button = new System.Windows.Forms.Button();
            this.up_button = new System.Windows.Forms.Button();
            this.right_button = new System.Windows.Forms.Button();
            this.hwgbox.SuspendLayout();
            this.ccgbox.SuspendLayout();
            this.SuspendLayout();
            // 
            // ip_connect_button
            // 
            this.ip_connect_button.Location = new System.Drawing.Point(197, 4);
            this.ip_connect_button.Name = "ip_connect_button";
            this.ip_connect_button.Size = new System.Drawing.Size(75, 23);
            this.ip_connect_button.TabIndex = 0;
            this.ip_connect_button.Text = "Connect";
            this.ip_connect_button.UseVisualStyleBackColor = true;
            this.ip_connect_button.Click += new System.EventHandler(this.ip_connect_button_Click);
            // 
            // ip_label
            // 
            this.ip_label.AutoSize = true;
            this.ip_label.Location = new System.Drawing.Point(12, 9);
            this.ip_label.Name = "ip_label";
            this.ip_label.Size = new System.Drawing.Size(61, 13);
            this.ip_label.TabIndex = 1;
            this.ip_label.Text = "IP Address:";
            // 
            // ip_disconnect_button
            // 
            this.ip_disconnect_button.Location = new System.Drawing.Point(278, 4);
            this.ip_disconnect_button.Name = "ip_disconnect_button";
            this.ip_disconnect_button.Size = new System.Drawing.Size(75, 23);
            this.ip_disconnect_button.TabIndex = 3;
            this.ip_disconnect_button.Text = "Disconnect";
            this.ip_disconnect_button.UseVisualStyleBackColor = true;
            this.ip_disconnect_button.Click += new System.EventHandler(this.ip_disconnect_button_Click);
            // 
            // left_button
            // 
            this.left_button.Font = new System.Drawing.Font("Verdana", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.left_button.Location = new System.Drawing.Point(6, 72);
            this.left_button.Name = "left_button";
            this.left_button.Size = new System.Drawing.Size(112, 42);
            this.left_button.TabIndex = 4;
            this.left_button.Text = "Left";
            this.left_button.UseVisualStyleBackColor = true;
            this.left_button.Click += new System.EventHandler(this.left_button_Click);
            // 
            // hwgbox
            // 
            this.hwgbox.Controls.Add(this.hw_init);
            this.hwgbox.Controls.Add(this.hw_real_mode);
            this.hwgbox.Controls.Add(this.hw_sim_mode);
            this.hwgbox.Controls.Add(this.hw_status_label);
            this.hwgbox.Controls.Add(this.hw_mode_label);
            this.hwgbox.Controls.Add(this.hw_list);
            this.hwgbox.Enabled = false;
            this.hwgbox.Location = new System.Drawing.Point(12, 42);
            this.hwgbox.Name = "hwgbox";
            this.hwgbox.Size = new System.Drawing.Size(274, 274);
            this.hwgbox.TabIndex = 5;
            this.hwgbox.TabStop = false;
            this.hwgbox.Text = "Hardware";
            // 
            // hw_init
            // 
            this.hw_init.BackColor = System.Drawing.SystemColors.Control;
            this.hw_init.Enabled = false;
            this.hw_init.ForeColor = System.Drawing.SystemColors.ControlText;
            this.hw_init.Location = new System.Drawing.Point(50, 198);
            this.hw_init.Name = "hw_init";
            this.hw_init.Size = new System.Drawing.Size(75, 23);
            this.hw_init.TabIndex = 11;
            this.hw_init.Text = "Initialized";
            this.hw_init.UseVisualStyleBackColor = false;
            this.hw_init.Click += new System.EventHandler(this.hw_init_Click);
            // 
            // hw_real_mode
            // 
            this.hw_real_mode.BackColor = System.Drawing.SystemColors.Control;
            this.hw_real_mode.Enabled = false;
            this.hw_real_mode.ForeColor = System.Drawing.SystemColors.ControlText;
            this.hw_real_mode.Location = new System.Drawing.Point(131, 156);
            this.hw_real_mode.Name = "hw_real_mode";
            this.hw_real_mode.Size = new System.Drawing.Size(75, 23);
            this.hw_real_mode.TabIndex = 10;
            this.hw_real_mode.Text = "Real";
            this.hw_real_mode.UseVisualStyleBackColor = false;
            this.hw_real_mode.Click += new System.EventHandler(this.hw_real_mode_Click);
            // 
            // hw_sim_mode
            // 
            this.hw_sim_mode.BackColor = System.Drawing.SystemColors.Control;
            this.hw_sim_mode.Enabled = false;
            this.hw_sim_mode.ForeColor = System.Drawing.SystemColors.ControlText;
            this.hw_sim_mode.Location = new System.Drawing.Point(50, 156);
            this.hw_sim_mode.Name = "hw_sim_mode";
            this.hw_sim_mode.Size = new System.Drawing.Size(75, 23);
            this.hw_sim_mode.TabIndex = 9;
            this.hw_sim_mode.Text = "Simulation";
            this.hw_sim_mode.UseVisualStyleBackColor = false;
            this.hw_sim_mode.Click += new System.EventHandler(this.hw_sim_mode_Click);
            // 
            // hw_status_label
            // 
            this.hw_status_label.AutoSize = true;
            this.hw_status_label.Location = new System.Drawing.Point(6, 203);
            this.hw_status_label.Name = "hw_status_label";
            this.hw_status_label.Size = new System.Drawing.Size(40, 13);
            this.hw_status_label.TabIndex = 8;
            this.hw_status_label.Text = "Status:";
            // 
            // hw_mode_label
            // 
            this.hw_mode_label.AutoSize = true;
            this.hw_mode_label.Location = new System.Drawing.Point(6, 161);
            this.hw_mode_label.Name = "hw_mode_label";
            this.hw_mode_label.Size = new System.Drawing.Size(37, 13);
            this.hw_mode_label.TabIndex = 7;
            this.hw_mode_label.Text = "Mode:";
            // 
            // hw_list
            // 
            this.hw_list.FormattingEnabled = true;
            this.hw_list.Location = new System.Drawing.Point(6, 19);
            this.hw_list.Name = "hw_list";
            this.hw_list.Size = new System.Drawing.Size(135, 121);
            this.hw_list.TabIndex = 6;
            this.hw_list.SelectedIndexChanged += new System.EventHandler(this.hw_list_SelectedIndexChanged);
            // 
            // ip_combo_box
            // 
            this.ip_combo_box.FormattingEnabled = true;
            this.ip_combo_box.Location = new System.Drawing.Point(70, 6);
            this.ip_combo_box.Name = "ip_combo_box";
            this.ip_combo_box.Size = new System.Drawing.Size(121, 21);
            this.ip_combo_box.TabIndex = 11;
            // 
            // ccgbox
            // 
            this.ccgbox.Controls.Add(this.home_button);
            this.ccgbox.Controls.Add(this.down_button);
            this.ccgbox.Controls.Add(this.up_button);
            this.ccgbox.Controls.Add(this.left_button);
            this.ccgbox.Controls.Add(this.right_button);
            this.ccgbox.Enabled = false;
            this.ccgbox.Location = new System.Drawing.Point(292, 48);
            this.ccgbox.Name = "ccgbox";
            this.ccgbox.Size = new System.Drawing.Size(364, 173);
            this.ccgbox.TabIndex = 12;
            this.ccgbox.TabStop = false;
            this.ccgbox.Text = "Camera Control";
            // 
            // home_button
            // 
            this.home_button.Font = new System.Drawing.Font("Verdana", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.home_button.Location = new System.Drawing.Point(126, 72);
            this.home_button.Name = "home_button";
            this.home_button.Size = new System.Drawing.Size(112, 42);
            this.home_button.TabIndex = 8;
            this.home_button.Text = "Home";
            this.home_button.UseVisualStyleBackColor = true;
            this.home_button.Click += new System.EventHandler(this.home_button_Click);
            // 
            // down_button
            // 
            this.down_button.Font = new System.Drawing.Font("Verdana", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.down_button.Location = new System.Drawing.Point(126, 120);
            this.down_button.Name = "down_button";
            this.down_button.Size = new System.Drawing.Size(112, 42);
            this.down_button.TabIndex = 7;
            this.down_button.Text = "Down";
            this.down_button.UseVisualStyleBackColor = true;
            this.down_button.Click += new System.EventHandler(this.down_button_Click);
            // 
            // up_button
            // 
            this.up_button.Font = new System.Drawing.Font("Verdana", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.up_button.Location = new System.Drawing.Point(126, 24);
            this.up_button.Name = "up_button";
            this.up_button.Size = new System.Drawing.Size(112, 42);
            this.up_button.TabIndex = 6;
            this.up_button.Text = "Up";
            this.up_button.UseVisualStyleBackColor = true;
            this.up_button.Click += new System.EventHandler(this.up_button_Click);
            // 
            // right_button
            // 
            this.right_button.Font = new System.Drawing.Font("Verdana", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.right_button.Location = new System.Drawing.Point(244, 72);
            this.right_button.Name = "right_button";
            this.right_button.Size = new System.Drawing.Size(112, 42);
            this.right_button.TabIndex = 5;
            this.right_button.Text = "Right";
            this.right_button.UseVisualStyleBackColor = true;
            this.right_button.Click += new System.EventHandler(this.right_button_Click);
            // 
            // NiRobot
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.ccgbox);
            this.Controls.Add(this.ip_combo_box);
            this.Controls.Add(this.hwgbox);
            this.Controls.Add(this.ip_disconnect_button);
            this.Controls.Add(this.ip_label);
            this.Controls.Add(this.ip_connect_button);
            this.Name = "NiRobot";
            this.Text = "NiRobot";
            this.Load += new System.EventHandler(this.NiRobot_Load);
            this.hwgbox.ResumeLayout(false);
            this.hwgbox.PerformLayout();
            this.ccgbox.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button ip_connect_button;
        private System.Windows.Forms.Label ip_label;
        private System.Windows.Forms.Button ip_disconnect_button;
        private System.Windows.Forms.Button left_button;
        private System.ComponentModel.BackgroundWorker backgroundWorker1;
        private System.Windows.Forms.GroupBox hwgbox;
        private System.Windows.Forms.ListBox hw_list;
        private System.Windows.Forms.Button hw_real_mode;
        private System.Windows.Forms.Button hw_sim_mode;
        private System.Windows.Forms.Label hw_status_label;
        private System.Windows.Forms.Label hw_mode_label;
        private System.Windows.Forms.ComboBox ip_combo_box;
        private System.Windows.Forms.GroupBox ccgbox;
        private System.Windows.Forms.Button down_button;
        private System.Windows.Forms.Button up_button;
        private System.Windows.Forms.Button right_button;
        private System.Windows.Forms.Button home_button;
        private System.Windows.Forms.Button hw_init;
    }
}

