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
            _client = new RaspiClient(pictureBox1, "tcp://192.168.200.25:5555", 640, 480, CodecType.H264);
            _client.start();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (_client != null)
            {
                _client.stop();
                _client.Dispose();
            }
        } 
    }
}