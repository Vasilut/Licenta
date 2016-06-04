using System;

namespace ANN
{
    #region SigmoidFunction
    public class SigmoidActivationFunction
    {
        public static double processValue(double x)
        {
            //1 / ( 1 + e^(-x))
            return 1.0 / (1.0 + Math.Pow(Math.E, -x));
        }
    }
    #endregion
}
