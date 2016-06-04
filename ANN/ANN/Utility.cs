using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ANN
{
    public class Utility
    {
        //return the maximum number from a vector and the position
        public static Tuple<double,int> GetMaxim(double[] vector, int dim)
        {
            int pozMax = -1;
            double maxx = 0.0;
            for (int i = 0; i < dim; ++i)
            {
                if (vector[i] > maxx)
                {
                    maxx = vector[i];
                    pozMax = i;
                }
            }

            return new Tuple<double, int>(maxx, pozMax);
        }
    }
}
