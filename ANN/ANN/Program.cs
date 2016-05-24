using System;
using System.Diagnostics;
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
            // ProcesInitiator.InitProcess();

            //ReadFile rd = new ReadFile();
            //NeuronalNetwork neuronalNetwork = new NeuronalNetwork(540, 10, 6);
            //TrainerAnn trainerAnn = new TrainerAnn(ref neuronalNetwork, rd);
            //trainerAnn.DoTrain(5000);
            //Console.WriteLine("Antrenare completa");

            //trainerAnn.TestRun(3000);
            Console.WriteLine("Press any key to continue");
            Console.ReadLine();
        }
    }
}
