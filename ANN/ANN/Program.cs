using System;

namespace ANN
{
    class Program
    {
        static void Main(string[] args)
        {
            ReadFile rd = new ReadFile();
            NeuronalNetwork neuronalNetwork = new NeuronalNetwork(540, 3, 3);
            TrainerAnn trainerAnn = new TrainerAnn(ref neuronalNetwork, rd);
            trainerAnn.DoTrain(10);

            var x = 2;

            Console.WriteLine("Press any key to continue");
            Console.ReadLine();
        }
    }
}
