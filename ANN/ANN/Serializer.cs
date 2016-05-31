using System.IO;
using System.Runtime.Serialization.Formatters.Binary;

namespace ANN
{
    public class Serializer
    {
        public static void SaveNetwork(string path, ref NeuronalNetwork neuronalNetwork)
        {
            FileStream fs = new FileStream(path, FileMode.Create);
            BinaryFormatter bf = new BinaryFormatter();
            bf.Serialize(fs, neuronalNetwork);
            fs.Close();
        }

        public static NeuronalNetwork LoadNetwork(string fileName)
        {
            NeuronalNetwork neuronalNet;
            FileStream FS = new FileStream(fileName, FileMode.Open);
            BinaryFormatter BF = new BinaryFormatter();
            neuronalNet = (NeuronalNetwork)BF.Deserialize(FS);
            FS.Close();

            return neuronalNet;
        }
    }
}
