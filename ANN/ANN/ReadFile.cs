using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ANN
{
    public class ReadFile
    {
        public List<Tuple<List<double>, double>> GetTextContains()
        {
            var listLines = new List<Tuple<List<double>, double>>();

            //var fileStream = new FileStream(@"C:\Users\Lucian\Documents\Visual Studio 2015\Projects\FirstOpenCV\FirstOpenCV\fisier.out", FileMode.Open, FileAccess.Read);
            var fileStream = new FileStream("fisier.out", FileMode.Open, FileAccess.Read);
            using (var stream = new StreamReader(fileStream, Encoding.UTF8))
            {
                int i = 0;
                string line;
                while ((line = stream.ReadLine()) != null)
                {
                    if (i > 0)
                    {
                        List<double> listNumbers = new List<double>();
                        double multime = GetMultime(i);
                        string[] numbers = line.Split(' ');
                        for (int j = 0; j < numbers.Length; ++j)
                        {
                            string nr = numbers[j];
                            if (nr != "")
                            {
                                double rezult;
                                if (double.TryParse(nr, out rezult) == true)
                                {
                                    listNumbers.Add(rezult);
                                }
                            }

                        }
                        if (i <= 40)
                        {
                            listLines.Add(new Tuple<List<double>, double>(listNumbers, multime));
                        }
                    }
                    i++;
                    //listLines.Add(line);
                }
            }

            return listLines;
        }

        private double GetMultime(int x)
        {
            if (x >= 1 && x <= 7)
                return 0.0;
            else
            if (x >= 8 && x <= 13)
                return 1.0;
            else
            if (x >= 14 && x <= 19)
                return 2.0;
            else
            if (x >= 20 && x <= 26)
                return 3.0;
            else
            if (x >= 27 && x <= 33)
                return 4.0;
            else
                return 5.0;
        }

        public static List<string> ReadResult()
        {

            List<string> predictii = new List<string>();
            var fileStream = new FileStream("predictie.txt", FileMode.Open, FileAccess.Read);
            using (var stream = new StreamReader(fileStream, Encoding.UTF8))
            {
                int i = 0;
                string line;
                while ((line = stream.ReadLine()) != null)
                {
                    if(line == "")
                    {
                        break;
                    }
                    predictii.Add(line);
                }
            }

            return predictii;
        }
    }
}
