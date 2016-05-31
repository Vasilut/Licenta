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
    public partial class GraficTest : Form
    {
        private Dictionary<double, List<int>> _obj;
        public GraficTest(Dictionary<double, List<int>> obj)
        {
            InitializeComponent();
            _obj = obj;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //minge
            int mingeSize = _obj[0].Count;
            int numberOfMinge = (from r in _obj[0] where r == 0 select r).ToList().Count;
            this.chart1.Series["Target"].Points.AddXY("Minge", mingeSize);
            this.chart1.Series["Output"].Points.AddXY("Minge", numberOfMinge);

            //bere
            int bereSize = _obj[1].Count;
            int numberOfBere = (from r in _obj[1] where r == 1 select r).ToList().Count;
            this.chart1.Series["Target"].Points.AddXY("Bere", bereSize);
            this.chart1.Series["Output"].Points.AddXY("Bere", numberOfBere);

            //rate
            int rateSize = _obj[2].Count;
            int numberOfRate = (from r in _obj[2] where r == 2 select r).ToList().Count;
            this.chart1.Series["Target"].Points.AddXY("Rate", rateSize);
            this.chart1.Series["Output"].Points.AddXY("Rate", numberOfRate);

            //caini
            int cainiSize = _obj[3].Count;
            int numberOfCaini = (from r in _obj[3] where r == 3 select r).ToList().Count;
            this.chart1.Series["Target"].Points.AddXY("Caini", cainiSize);
            this.chart1.Series["Output"].Points.AddXY("Caini", numberOfCaini);
        }
    }
}
