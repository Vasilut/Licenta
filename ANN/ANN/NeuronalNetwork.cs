using System;
using System.Collections.Generic;
using System.IO;

namespace ANN
{
    public class NeuronalNetwork
    {
        private int _inputNeuron;
        private int _hiddenNeuron;
        private int _outputNeuron;


        private double[,] _inputToHiddenWeight;
        private double[,] _hiddenToOutputWeight;

        private double[] _inputs;
        private double[] _hidden;
        private double[] _outputs;

        private double _learningRate = 0.3;
        private Random _generatorRand = new Random();


        public NeuronalNetwork(int inputNeuron, int hiddenNeuron, int outputNeuron)
        {
            _inputNeuron = inputNeuron;
            _hiddenNeuron = hiddenNeuron;
            _outputNeuron = outputNeuron;

            _inputToHiddenWeight = new double[_inputNeuron, _hiddenNeuron];
            _hiddenToOutputWeight = new double[_hiddenNeuron, _outputNeuron];

            _inputs = new double[_inputNeuron];
            _hidden = new double[_hiddenNeuron];
            _outputs = new double[_outputNeuron];
        }

        public double[,] HiddenToOutputWeight
        {
            get
            {
                return _hiddenToOutputWeight;
            }
            set
            {
                _hiddenToOutputWeight = value;
            }
        }

        public double[,] InputToHiddenWeight
        {
            get
            {
                return _inputToHiddenWeight;
            }

            set
            {
                _inputToHiddenWeight = value;
            }
        }

        public double LearningRate
        {
            get
            {
                return _learningRate;
            }
            set
            {
                _learningRate = value;
            }
        }

        public void InitializeNetwork()
        {
            // Set weights between input & hidden nodes.
            for (int i = 0; i < _inputNeuron; ++i)
            {
                for (int j = 0; j < _hiddenNeuron; ++j)
                {
                    //the values are between -2 2
                    _inputToHiddenWeight[i, j] = (_generatorRand.NextDouble() * 4) - 2;
                }
            }

            // Set weights between hidden & output nodes.
            for (int i = 0; i < _hiddenNeuron; ++i)
            {
                for (int j = 0; j < _outputNeuron; ++j)
                {
                    _hiddenToOutputWeight[i, j] = (_generatorRand.NextDouble() * 4) - 2;
                }
            }
        }

        public void Forward(List<double> trainingInputs, double target, int epoca)
        {
            //Load the initial input layer
            for (int i = 0; i < _inputNeuron; ++i)
            {
                _inputs[i] = trainingInputs[i];
            }

            //Forward to hidden nodes, and calculate activations in hidden layer
            for (int i = 0; i < _hiddenNeuron; ++i)
            {
                double sum = 0.0;
                for (int j = 0; j < _inputNeuron; ++j)
                {
                    sum += _inputs[j] * _inputToHiddenWeight[j, i];
                }
                _hidden[i] = SigmoidActivationFunction.processValue(sum);
            }

            // Forward to output nodes, and calculate activations in output layer
            for (int i = 0; i < _outputNeuron; ++i)
            {
                double sum = 0.0;
                for (int j = 0; j < _hiddenNeuron; ++j)
                {
                    sum += _hidden[j] * _hiddenToOutputWeight[j, i];
                }

                // pass the sum, through the activation function, Sigmoid in this case
                _outputs[i] = SigmoidActivationFunction.processValue(sum);
            }
            
            Console.WriteLine(_outputs[0] + " " + _outputs[1] + " " + _outputs[2] + " Target " + " " + target + " Epoca" + epoca );
        }

        public int InputNeuron { get { return _inputNeuron; } set { _inputNeuron = value; } }
        public int HiddenNeuron { get { return _hiddenNeuron; } set { _hiddenNeuron = value; } }
        public int OutputNeuron { get { return _outputNeuron; } set { _outputNeuron = value; } }

        public double[] Inputs { get { return _inputs; } set { _inputs = value; } }
        public double[] Hidden { get { return _hidden; } set { _hidden = value; } }
        public double[] Outputs { get { return _outputs; } set { _outputs = value; } }

    }
}
