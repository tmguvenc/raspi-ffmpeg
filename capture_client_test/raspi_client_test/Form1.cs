using System;
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
            _client = new RaspiClient(pictureBox1, "tcp://192.168.200.53:5555", 1920, 1080, CodecType.H264);
            _client.start();
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
    }
}