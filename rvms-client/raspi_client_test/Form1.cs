using System;
using System.Drawing;
using System.Globalization;
using System.Windows.Forms;
using Client;

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

        private void button1_Click(object sender, EventArgs e)
        {
            _client = new RaspiClient(pictureBox1, txt_ip.Text, Convert.ToUInt16(txt_port.Text));
            _client.OnSensorDataReceived += _client_OnSensorDataReceived;
            _client.start();
        }

        private void _client_OnSensorDataReceived(float arg1, float arg2)
        {
            lbl_humidity.Text = arg1.ToString(CultureInfo.CurrentCulture);
            lbl_temperature.Text = arg2.ToString(CultureInfo.CurrentCulture);
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

        private void btn_readSensor_Click(object sender, EventArgs e)
        {
            _client.readSensor();
        } 
    }
}