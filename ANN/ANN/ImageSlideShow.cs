using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace ANN
{
    public partial class ImageSlideShow : Form
    {
        int index = 1;
        private List<string> _listaPredictii;
        public ImageSlideShow(string nameOfObject,List<string> listaPredictii)
        {
            InitializeComponent();
            _listaPredictii = listaPredictii;
            
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            //the time is ticking
            //showing a picture
            if (index < _listaPredictii.Count)
            {
                pictureBox1.Image = Image.FromFile(_listaPredictii[index++]);
            }
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {
            timer1.Enabled = true;
        }
    }
}
