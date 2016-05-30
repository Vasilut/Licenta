using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ANN
{
    public partial class RunApp : Form
    {
        public RunApp()
        {
            InitializeComponent();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            ProcesInitiator.InitProcess("2");
        }

        private void button3_Click(object sender, EventArgs e)
        {
            //read the pictures from db, make the xml format with pictures descriptor and create the file with this descriptors
            //we need to run the c++ open cv app
            ProcesInitiator.InitProcess("1");

        }

        private void button4_Click(object sender, EventArgs e)
        {
            OpenFileDialog fileDialog = new OpenFileDialog();
            fileDialog.Title = "Please select a picture";
            fileDialog.Multiselect = false;
            fileDialog.Filter = "PNG|*.png";
            DialogResult dr = fileDialog.ShowDialog();
            if( dr == DialogResult.OK)
            {
                string fileName = Path.GetFileName(fileDialog.FileName);
                string parameter = "4 " + fileName;
                ProcesInitiator.InitProcess(parameter);
                var listaPredictii = ReadFile.ReadListaPozeAsociate();
                new ImageCategory(listaPredictii[0], listaPredictii).Show();
                new ImageSlideShow(listaPredictii[0], listaPredictii).Show();
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
           
        }
    }
}
