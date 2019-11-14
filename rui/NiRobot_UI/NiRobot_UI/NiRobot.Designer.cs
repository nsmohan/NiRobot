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
            this.ip_textbox = new System.Windows.Forms.TextBox();
            this.ip_disconnect_button = new System.Windows.Forms.Button();
            this.left_button = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // ip_connect_button
            // 
            this.ip_connect_button.Location = new System.Drawing.Point(185, 6);
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
            // ip_textbox
            // 
            this.ip_textbox.Location = new System.Drawing.Point(79, 6);
            this.ip_textbox.Name = "ip_textbox";
            this.ip_textbox.Size = new System.Drawing.Size(100, 20);
            this.ip_textbox.TabIndex = 2;
            // 
            // ip_disconnect_button
            // 
            this.ip_disconnect_button.Location = new System.Drawing.Point(266, 6);
            this.ip_disconnect_button.Name = "ip_disconnect_button";
            this.ip_disconnect_button.Size = new System.Drawing.Size(75, 23);
            this.ip_disconnect_button.TabIndex = 3;
            this.ip_disconnect_button.Text = "Disconnect";
            this.ip_disconnect_button.UseVisualStyleBackColor = true;
            this.ip_disconnect_button.Click += new System.EventHandler(this.ip_disconnect_button_Click);
            // 
            // left_button
            // 
            this.left_button.Location = new System.Drawing.Point(201, 72);
            this.left_button.Name = "left_button";
            this.left_button.Size = new System.Drawing.Size(75, 23);
            this.left_button.TabIndex = 4;
            this.left_button.Text = "Left";
            this.left_button.UseVisualStyleBackColor = true;
            this.left_button.Click += new System.EventHandler(this.left_button_Click);
            // 
            // NiRobot
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.left_button);
            this.Controls.Add(this.ip_disconnect_button);
            this.Controls.Add(this.ip_textbox);
            this.Controls.Add(this.ip_label);
            this.Controls.Add(this.ip_connect_button);
            this.Name = "NiRobot";
            this.Text = "NiRobot";
            this.Load += new System.EventHandler(this.NiRobot_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button ip_connect_button;
        private System.Windows.Forms.Label ip_label;
        private System.Windows.Forms.TextBox ip_textbox;
        private System.Windows.Forms.Button ip_disconnect_button;
        private System.Windows.Forms.Button left_button;
    }
}

