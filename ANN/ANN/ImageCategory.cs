using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ANN
{
    public partial class ImageCategory : Form
    {
        public ImageCategory(string word, List<string> predictii)
        {
            InitializeComponent();
            label4.Text = word;
            buildGalery(predictii);
        }

        private void buildGalery(List<string> listaPredictii)
        {
            int x = 40;
            int y = 80;
            int maxHeight = -1;

            for (int i = 1; i < listaPredictii.Count; ++i)
            {
                string pictureUrl = listaPredictii[i];
                PictureBox pic = new PictureBox();
                pic.Image = Image.FromFile(pictureUrl);
                pic.Location = new Point(x, y);
                pic.SizeMode = PictureBoxSizeMode.StretchImage;
                x += pic.Width + 10;
                maxHeight = Math.Max(pic.Height, maxHeight);
                if (x > this.ClientSize.Width - 100)
                {
                    x = 20;
                    y += maxHeight + 10;
                }

                this.panel1.Controls.Add(pic);
            }
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }
    }
}
