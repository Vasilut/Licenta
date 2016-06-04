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
        // read the training data from file
        public List<Tuple<List<double>, double>> GetTrainingData()
        {
            var listLines = new List<Tuple<List<double>, double>>();

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
                        if (i <= 144) // normal e <=180, pt 5 clase
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

        //read the description of a picture for the ann implementation
        public List<double> GetPictureForAnn()
        {
            var listLines = new List<double>();

            var fileStream = new FileStream("annfeature.out", FileMode.Open, FileAccess.Read);
            using (var stream = new StreamReader(fileStream, Encoding.UTF8))
            {
                string line;
                while ((line = stream.ReadLine()) != null)
                {
                    string[] numbers = line.Split(' ');
                    for (int j = 0; j < numbers.Length; ++j)
                    {
                        string nr = numbers[j];
                        if (nr != "")
                        {
                            double rezult;
                            if (double.TryParse(nr, out rezult) == true)
                            {
                                listLines.Add(rezult);
                            }
                        }

                    }
                }
            }

            return listLines;
        }

        //return the number coresponding to a photo for training data
        private double GetMultime(int x)
        {
            if (x >= 1 && x <= 36)
                return 0.0;
            else
            if (x >= 37 && x <= 72)
                return 1.0;
            else
            if (x >= 73 && x <= 108)
                return 2.0;
            else
            if (x >= 109 && x <= 144)
                return 3.0;
            else
                return 4.0;
        }

        //read the name of the pictures which are in the same category like the uploaded photo
        public static List<string> ReadListaPozeAsociate()
        {

            List<string> predictii = new List<string>();
            var fileStream = new FileStream("predictie.txt", FileMode.Open, FileAccess.Read);
            using (var stream = new StreamReader(fileStream, Encoding.UTF8))
            {
                int i = 0;
                string line;
                while ((line = stream.ReadLine()) != null)
                {
                    if (line == "")
                    {
                        break;
                    }
                    predictii.Add(line);
                }
            }

            return predictii;
        }

        //read the test data
        public List<Tuple<List<double>, double>> GetTestData()
        {
            var listLines = new List<Tuple<List<double>, double>>();

            var fileStream = new FileStream("datetest.out", FileMode.Open, FileAccess.Read);
            using (var stream = new StreamReader(fileStream, Encoding.UTF8))
            {
                int i = 0;
                string line;
                while ((line = stream.ReadLine()) != null)
                {
                    if (i > 0)
                    {
                        List<double> listNumbers = new List<double>();
                        double multime = GetMultimeTest(i);
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
                        if (i <= 14) // normal e <=16, pentru toate clasele
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

        //return the number coresponding to a photo for test data
        private double GetMultimeTest(int x)
        {
            if (x >= 1 && x <= 4)
            {
                return 2.0;
            }
            else
            if (x >= 5 && x <= 8)
            {
                return 0.0;
            }
            else
            if (x >= 9 && x <= 12)
            {
                return 1.0;
            }
            else
            if (x >= 13 && x <= 14)
            {
                return 3.0;
            }
            else
                return 4.0;
        }
    }
}
