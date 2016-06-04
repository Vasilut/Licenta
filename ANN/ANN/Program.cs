using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
using System.Windows.Forms;

namespace ANN
{
    class Program
    {
        [STAThread]
        static void Main(string[] args)
        {

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new RunApp());
            //ProcesInitiator.InitProcess();

            //ReadFile rd = new ReadFile();
            //NeuronalNetwork neuronalNetwork = new NeuronalNetwork(540, 10, 4);
            //TrainerAnn trainerAnn = new TrainerAnn(ref neuronalNetwork, rd, false);
            //trainerAnn.DoTrain(5000);
            //Serializer.SaveNetwork("BigNetwork.net", ref neuronalNetwork);
            //Console.WriteLine("Antrenare completa");
            
            ////NeuronalNetwork nr = Serializer.LoadNetwork("serializat.net");
            ////TrainerAnn trainerAnn = new TrainerAnn(ref nr, rd, true);
            //trainerAnn.TestRun(3000);
            //Console.WriteLine("Press any key to continue");
            //Console.ReadLine();
            
        }
    }
}
