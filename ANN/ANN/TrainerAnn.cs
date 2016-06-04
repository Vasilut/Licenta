using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;

namespace ANN
{
    public class TrainerAnn
    {
        private NeuronalNetwork _neuronalNetwork;
        private Random _randGen = new Random();
        private int _trainingTimes = 10000;
        private List<Tuple<List<double>, double>> _trainSet = new List<Tuple<List<double>, double>>();
        private List<Tuple<List<double>, double>> _testData = new List<Tuple<List<double>, double>>();
        private Dictionary<double, List<int>> _dict = new Dictionary<double, List<int>>()
        {
            [0] = new List<int>(),
            [1] = new List<int>(),
            [2] = new List<int>(),
            [3] = new List<int>()
        };

        public TrainerAnn(ref NeuronalNetwork neuronalNetwork, ReadFile rf, bool serialize)
        {
            _neuronalNetwork = neuronalNetwork;
            if (serialize == false)
            {
                _neuronalNetwork.InitializeNetwork();
            }
            //initializa training data
            _trainSet = rf.GetTrainingData();
            _testData = rf.GetTestData();
            File.WriteAllText("pua.txt", string.Empty);
        }

        public void DoTrain(int trainingTimes)
        {
            _trainingTimes = trainingTimes;
            //loop for n-many training times
            for (int i = 0; i < _trainingTimes; ++i)
            {
                for (int j = 0; j < _trainSet.Count; ++j)
                {
                    // Train using example set
                    List<double> values = _trainSet[j].Item1;
                    //feed forward through network
                    _neuronalNetwork.Forward(values, _trainSet[j].Item2, i);

                    //back propagation
                    BackPropagation(_trainSet[j].Item2);
                }
            }
        }

        public Dictionary<double, List<int>> DictionaryTestData
        {
            get
            {
                return _dict;
            }
            set
            {
                _dict = value;
            }
        }

        public void TestRun(int trainingTimes)
        {
            //run the forward propagation for test data

            _trainingTimes = trainingTimes;
            //for each picture we'll take out from the dataset, we'll train the network without this data set
            //and then we'll verify with this input
            for (int indicePoze = 0; indicePoze < _testData.Count; ++indicePoze)
            {
                List<double> values = _testData[indicePoze].Item1;

                //cross over
                //for (int i = 0; i < _trainingTimes; ++i)
                //{
                //    for (int j = 0; j < _trainSet.Count; ++j)
                //    {
                //        if( j == indicePoze)
                //        {
                //            continue;
                //        }
                //        // Train using example set
                //        List<double> valori = _trainSet[j].Item1;
                //        //feed forward through network
                //        _neuronalNetwork.Forward(valori, _trainSet[j].Item2, i);
                //        //do the weight changes (pass back)
                //        TrainNetwork(_trainSet[j].Item2);
                //    }
                //}

                _neuronalNetwork.Forward(values, _testData[indicePoze].Item2, 0);

                Tuple<double, int> rezultMax = Utility.GetMaxim(_neuronalNetwork.Outputs, _neuronalNetwork.OutputNeuron);
                int pozMax = rezultMax.Item2;
                double maxx = rezultMax.Item1;

                _dict[_testData[indicePoze].Item2].Add(pozMax);
                using (var wr = new StreamWriter("pua.txt", true))
                {
                    wr.WriteLine(_neuronalNetwork.Outputs[0] + " " + _neuronalNetwork.Outputs[1] + " " +
                                _neuronalNetwork.Outputs[2]
                                + " " + _neuronalNetwork.Outputs[3] + " "/* + _neuronalNetwork.Outputs[4]*/
                                + " " + " Target " + " " + (_testData[indicePoze].Item2)
                                + " Maximul: " + maxx + " Poz Maxima : " + pozMax)
                                ;
                    wr.WriteLine("---------------------------");
                }
            }


        }

        public int ForwardPropagation(List<double> values)
        {
            //forward propagation just for a set of values
            _neuronalNetwork.Forward(values, 0, 0);

            Tuple<double, int> rezultMax = Utility.GetMaxim(_neuronalNetwork.Outputs, _neuronalNetwork.OutputNeuron);
            int pozMax = rezultMax.Item2;
            double maxx = rezultMax.Item1;
            return pozMax;
        }

        private void BackPropagation(double multime)
        {
            //get momentum values (delta values from last pass)
            double[] delta_hidden = new double[_neuronalNetwork.HiddenNeuron];
            double[] delta_outputs = new double[_neuronalNetwork.OutputNeuron];

            // Get the delta value for the output layer
            for (int i = 0; i < _neuronalNetwork.OutputNeuron; ++i)
            {
                double target = 0.0;
                if (i == multime)
                {
                    target = 1.0;
                }
                delta_outputs[i] =
                    _neuronalNetwork.Outputs[i] * (1.0 - _neuronalNetwork.Outputs[i]) * (target - _neuronalNetwork.Outputs[i]);
            }

            // Get the delta value for the hidden layer
            for (int i = 0; i < _neuronalNetwork.HiddenNeuron; ++i)
            {
                double error = 0.0;
                for (int j = 0; j < _neuronalNetwork.OutputNeuron; ++j)
                {
                    error += _neuronalNetwork.HiddenToOutputWeight[i, j] * delta_outputs[j];
                }
                delta_hidden[i] = _neuronalNetwork.Hidden[i] * (1.0 - _neuronalNetwork.Hidden[i]) * error;
            }

            // Now update the weights between hidden & output layer
            for (int i = 0; i < _neuronalNetwork.OutputNeuron; ++i)
            {
                for (int j = 0; j < _neuronalNetwork.HiddenNeuron; ++j)
                {
                    //use momentum (delta values from last pass), 
                    //to ensure moved in correct direction
                    _neuronalNetwork.HiddenToOutputWeight[j, i] += _neuronalNetwork.LearningRate * delta_outputs[i] * _neuronalNetwork.Hidden[j];
                }
            }

            // Now update the weights between input & hidden layer
            for (int i = 0; i < _neuronalNetwork.HiddenNeuron; ++i)
            {
                for (int j = 0; j < _neuronalNetwork.InputNeuron; ++j)
                {
                    //use momentum (delta values from last pass), 
                    //to ensure moved in correct direction
                    _neuronalNetwork.InputToHiddenWeight[j, i] += _neuronalNetwork.LearningRate * delta_hidden[i] * _neuronalNetwork.Inputs[j];
                }
            }

        }

    }
}
