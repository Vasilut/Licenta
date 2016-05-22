using System;
using System.Collections.Generic;

namespace ANN
{
    public class TrainerAnn
    {
        private NeuronalNetwork _neuronalNetwork;
        private Random _randGen = new Random();
        private int _trainingTimes = 10000;
        private List<Tuple<List<double>, double>> _trainSet = new List<Tuple<List<double>, double>>();

        public TrainerAnn(ref NeuronalNetwork neuronalNetwork, ReadFile rf)
        {
            _neuronalNetwork = neuronalNetwork;
            _neuronalNetwork.InitializeNetwork();
            _trainSet = rf.GetTextContains();
        }
        
        public void DoTrain(int trainingTimes)
        {
            _trainingTimes = trainingTimes;
            //loop for n-many training times
            for (int i = 0; i < _trainingTimes; ++i)
            {
                for(int j = 0; j < _trainSet.Count; ++j)
                {
                    // Train using example set
                    List<double> values = _trainSet[j].Item1;
                    //feed forward through network
                    _neuronalNetwork.Forward(values, _trainSet[j].Item2);
                    //do the weight changes (pass back)
                    TrainNetwork(_trainSet[j].Item2);
                }
            }
        }

        private void TrainNetwork(double multime)
        {
            //get momentum values (delta values from last pass)
            double[] delta_hidden = new double[_neuronalNetwork.HiddenNeuron];
            double[] delta_outputs = new double[_neuronalNetwork.OutputNeuron];

            // Get the delta value for the output layer
            for(int i = 0; i < _neuronalNetwork.OutputNeuron; ++i)
            {
                delta_outputs[i] =
                    _neuronalNetwork.Outputs[i] * (1.0 - _neuronalNetwork.Outputs[i]) * (multime - _neuronalNetwork.Outputs[i]);
            }

            // Get the delta value for the hidden layer
            for(int i = 0; i < _neuronalNetwork.HiddenNeuron; ++i)
            {
                double error = 0.0;
                for(int j = 0; j < _neuronalNetwork.OutputNeuron; ++j)
                {
                    error += _neuronalNetwork.HiddenToOutputWeight[i, j] * delta_outputs[j];
                }
                delta_hidden[i] = _neuronalNetwork.Hidden[i] * (1.0 - _neuronalNetwork.Hidden[i]) * error;
            }

            // Now update the weights between hidden & output layer
            for(int i = 0; i < _neuronalNetwork.OutputNeuron; ++i)
            {
                for(int j = 0; j < _neuronalNetwork.HiddenNeuron; ++j)
                {
                    //use momentum (delta values from last pass), 
                    //to ensure moved in correct direction
                    _neuronalNetwork.HiddenToOutputWeight[j, i] += _neuronalNetwork.LearningRate * delta_outputs[i] * _neuronalNetwork.Hidden[j];
                }
            }

            // Now update the weights between input & hidden layer
            for(int i = 0; i < _neuronalNetwork.HiddenNeuron; ++i)
            {
                for(int j = 0; j < _neuronalNetwork.InputNeuron; ++j)
                {
                    //use momentum (delta values from last pass), 
                    //to ensure moved in correct direction
                    _neuronalNetwork.InputToHiddenWeight[j, i] += _neuronalNetwork.LearningRate * delta_hidden[i] * _neuronalNetwork.Inputs[j];
                }
            }

        }

    }
}
