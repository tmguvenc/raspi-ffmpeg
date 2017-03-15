using System;
using System.Globalization;
using System.Windows.Forms;
using Client;
using Timer = System.Timers.Timer;

namespace raspi_client_test
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            RaspiClient.initialize();
        }

        RaspiClient _client;
        private Timer _timer;

        private void button1_Click(object sender, EventArgs e)
        {
            _client = new RaspiClient(pictureBox1, txt_ip.Text, Convert.ToUInt16(txt_port.Text));
            _client.OnSensorDataReceived += _client_OnSensorDataReceived;

            _timer = new Timer(10000);

            _timer.Elapsed += (o, args) =>
            {
                _client.readSensor();
            };

            _client.start();
            _timer.Start();

        }

        private void _client_OnSensorDataReceived(float arg1, float arg2)
        {
            if (lbl_humidity.InvokeRequired)
            {
                BeginInvoke(new Action(() =>
                {
                    lbl_humidity.Text = string.Format("Humdity: {0}%", arg1.ToString(CultureInfo.CurrentCulture));
                    lbl_temperature.Text = string.Format("Temperature: {0} °C", arg2.ToString(CultureInfo.CurrentCulture));
                }));
            }
        }

        private void Stop()
        {
            if (_client != null)
            {
                _client.stop();
                _client.Dispose();
                _client = null;
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Stop();    
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            Stop();
        }

        private void btn_up_MouseUp(object sender, MouseEventArgs e)
        {
            _client.motorStop();
        }

        private void btn_up_MouseClick(object sender, MouseEventArgs e)
        {
            _client.moveUp();
        }

        private void btn_right_MouseClick(object sender, MouseEventArgs e)
        {
            _client.moveRight();
        }

        private void btl_left_Click(object sender, EventArgs e)
        {
            _client.moveLeft();
        }

        private void btn_down_Click(object sender, EventArgs e)
        {
            _client.moveDown();
        }
    }
}