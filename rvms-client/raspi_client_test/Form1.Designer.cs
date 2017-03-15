namespace raspi_client_test
{
    partial class Form1
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
            this.button1 = new System.Windows.Forms.Button();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.button2 = new System.Windows.Forms.Button();
            this.txt_ip = new System.Windows.Forms.TextBox();
            this.txt_port = new System.Windows.Forms.TextBox();
            this.lbl_humidity = new System.Windows.Forms.Label();
            this.lbl_temperature = new System.Windows.Forms.Label();
            this.btn_up = new System.Windows.Forms.Button();
            this.btl_left = new System.Windows.Forms.Button();
            this.btn_right = new System.Windows.Forms.Button();
            this.btn_down = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(324, 12);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 0;
            this.button1.Text = "Start";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // pictureBox1
            // 
            this.pictureBox1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pictureBox1.Location = new System.Drawing.Point(22, 41);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(1280, 960);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox1.TabIndex = 1;
            this.pictureBox1.TabStop = false;
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(405, 12);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 2;
            this.button2.Text = "Stop";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // txt_ip
            // 
            this.txt_ip.Location = new System.Drawing.Point(22, 13);
            this.txt_ip.Name = "txt_ip";
            this.txt_ip.Size = new System.Drawing.Size(162, 20);
            this.txt_ip.TabIndex = 3;
            this.txt_ip.Text = "192.168.1.140";
            // 
            // txt_port
            // 
            this.txt_port.Location = new System.Drawing.Point(191, 12);
            this.txt_port.Name = "txt_port";
            this.txt_port.Size = new System.Drawing.Size(63, 20);
            this.txt_port.TabIndex = 4;
            this.txt_port.Text = "5555";
            // 
            // lbl_humidity
            // 
            this.lbl_humidity.AutoSize = true;
            this.lbl_humidity.Location = new System.Drawing.Point(626, 12);
            this.lbl_humidity.Name = "lbl_humidity";
            this.lbl_humidity.Size = new System.Drawing.Size(0, 13);
            this.lbl_humidity.TabIndex = 5;
            // 
            // lbl_temperature
            // 
            this.lbl_temperature.AutoSize = true;
            this.lbl_temperature.Location = new System.Drawing.Point(837, 13);
            this.lbl_temperature.Name = "lbl_temperature";
            this.lbl_temperature.Size = new System.Drawing.Size(0, 13);
            this.lbl_temperature.TabIndex = 6;
            // 
            // btn_up
            // 
            this.btn_up.Location = new System.Drawing.Point(1472, 41);
            this.btn_up.Name = "btn_up";
            this.btn_up.Size = new System.Drawing.Size(41, 24);
            this.btn_up.TabIndex = 7;
            this.btn_up.Text = "Up";
            this.btn_up.UseVisualStyleBackColor = true;
            this.btn_up.MouseClick += new System.Windows.Forms.MouseEventHandler(this.btn_up_MouseClick);
            this.btn_up.MouseUp += new System.Windows.Forms.MouseEventHandler(this.btn_up_MouseUp);
            // 
            // btl_left
            // 
            this.btl_left.Location = new System.Drawing.Point(1425, 71);
            this.btl_left.Name = "btl_left";
            this.btl_left.Size = new System.Drawing.Size(41, 24);
            this.btl_left.TabIndex = 8;
            this.btl_left.Text = "Left";
            this.btl_left.UseVisualStyleBackColor = true;
            this.btl_left.Click += new System.EventHandler(this.btl_left_Click);
            this.btl_left.MouseUp += new System.Windows.Forms.MouseEventHandler(this.btn_up_MouseUp);
            // 
            // btn_right
            // 
            this.btn_right.Location = new System.Drawing.Point(1519, 71);
            this.btn_right.Name = "btn_right";
            this.btn_right.Size = new System.Drawing.Size(41, 24);
            this.btn_right.TabIndex = 9;
            this.btn_right.Text = "Right";
            this.btn_right.UseVisualStyleBackColor = true;
            this.btn_right.MouseClick += new System.Windows.Forms.MouseEventHandler(this.btn_right_MouseClick);
            this.btn_right.MouseUp += new System.Windows.Forms.MouseEventHandler(this.btn_up_MouseUp);
            // 
            // btn_down
            // 
            this.btn_down.Location = new System.Drawing.Point(1469, 101);
            this.btn_down.Name = "btn_down";
            this.btn_down.Size = new System.Drawing.Size(47, 24);
            this.btn_down.TabIndex = 10;
            this.btn_down.Text = "Down";
            this.btn_down.UseVisualStyleBackColor = true;
            this.btn_down.Click += new System.EventHandler(this.btn_down_Click);
            this.btn_down.MouseUp += new System.Windows.Forms.MouseEventHandler(this.btn_up_MouseUp);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1652, 1029);
            this.Controls.Add(this.btn_down);
            this.Controls.Add(this.btn_right);
            this.Controls.Add(this.btl_left);
            this.Controls.Add(this.btn_up);
            this.Controls.Add(this.lbl_temperature);
            this.Controls.Add(this.lbl_humidity);
            this.Controls.Add(this.txt_port);
            this.Controls.Add(this.txt_ip);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.button1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.TextBox txt_ip;
        private System.Windows.Forms.TextBox txt_port;
        private System.Windows.Forms.Label lbl_humidity;
        private System.Windows.Forms.Label lbl_temperature;
        private System.Windows.Forms.Button btn_up;
        private System.Windows.Forms.Button btl_left;
        private System.Windows.Forms.Button btn_right;
        private System.Windows.Forms.Button btn_down;
    }
}

