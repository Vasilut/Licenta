using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace ANN
{
    public partial class ImageNetwork : Form
    {
        private ReadFile _readFile;
        private NeuronalNetwork _neuronalNetwork = null;
        private TrainerAnn _trainAnn;
        private Dictionary<int, string> classes = new Dictionary<int, string>()
        {
            [0] = "minge",
            [1] = "bere",
            [2] = "rata",
            [3] = "caine"
        };

        public ImageNetwork()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            // browse for a picture
            OpenFileDialog fileDialog = new OpenFileDialog();
            fileDialog.Title = "Please select a picture";
            fileDialog.Multiselect = false;
            fileDialog.Filter = "PNG|*.png";
            DialogResult dr = fileDialog.ShowDialog();
            if (dr == DialogResult.OK)
            {
                string fileName = Path.GetFileName(fileDialog.FileName);
                string path = fileDialog.FileName;
                textBox1.Text = path;
                pictureBox1.Image = Image.FromFile(path);
                string parameter = "5 " + fileName;
                ProcesInitiator.InitProcess(parameter);
                button4.Enabled = true; //activam butonul de recognize
            }
        }

        private void button6_Click(object sender, EventArgs e)
        {
            //clear button
            textBox1.Text = "";
            pictureBox1.Image = null;
            label4.Text = "";
        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {

        }

        private void ImageNetwork_Load(object sender, EventArgs e)
        {
            _readFile = new ReadFile();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            //train network 
            listBox1.Items.Add("Training begin...\n");

            //the network was not loaded, is new, so we need to train
            _neuronalNetwork = new NeuronalNetwork(540, 10, 4);
            _trainAnn = new TrainerAnn(ref _neuronalNetwork, _readFile, false);
            _trainAnn.DoTrain(5000);
            listBox1.Items.Add("Training complete...\n");

            //_trainAnn.TestRun(3000);
            //listBox1.Text += "Testing data complete...\n";
            
        }

        private void button2_Click(object sender, EventArgs e)
        {
            //load neuronal network from file
            using (var fileDialog = new OpenFileDialog())
            {
                fileDialog.Title = "Please select a file";
                fileDialog.Multiselect = false;
                fileDialog.Filter = "NET|*.net";
                DialogResult dr = fileDialog.ShowDialog();
                if (dr == DialogResult.OK)
                {
                    string fileName = Path.GetFileName(fileDialog.FileName);
                    _neuronalNetwork = Serializer.LoadNetwork(fileName);
                    _trainAnn = new TrainerAnn(ref _neuronalNetwork, _readFile, true);
                    listBox1.Items.Add("Load Network complete...\n");
                    button5.Enabled = true; // dam enable la butonul de save
                }
            }
            
        }

        private void button5_Click(object sender, EventArgs e)
        {
            //save current configuration
            using (SaveFileDialog saveDialog = new SaveFileDialog())
            {
                saveDialog.Filter = "Network File(*.net)|*.net";
                if(saveDialog.ShowDialog() == DialogResult.OK)
                {
                    string path = Path.GetFileName(saveDialog.FileName);
                    Serializer.SaveNetwork(path, ref _neuronalNetwork);
                    listBox1.Items.Add("Network successfully saved...\n");
                }

            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            //recognize picture
            //we read the file annfeauture.out for reading the features for the image
            List<double> features = _readFile.GetPictureForAnn();
            int result = _trainAnn.ForwardPropagation(features);
            string clasa = classes[result];
            label4.Text = clasa;

        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}
