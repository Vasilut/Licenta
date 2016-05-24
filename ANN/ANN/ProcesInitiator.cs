using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ANN
{
    public class ProcesInitiator
    {
        public static void InitProcess(string arguments)
        {
            var proc = new Process();

            proc.StartInfo.FileName = @"C:\Users\Lucian\Documents\Visual Studio 2015\Projects\FirstOpenCV\x64\Debug\FirstOpenCV.exe";
            proc.StartInfo.UseShellExecute = true;
            //proc.StartInfo.RedirectStandardInput = true;
            proc.StartInfo.CreateNoWindow = false;
            proc.StartInfo.Arguments = arguments;
            //proc.StartInfo.Arguments = "\"C:\\Users\\Lucian\\Documents\\Visual Studio 2015\\Projects\\OpenCVHogDescriptor\\pictures\\testData\\pic9.jpg\"";
            proc.Start();
            //var sw = proc.StandardInput;
            //sw.Write('1');


            proc.WaitForExit();
            proc.Close();
        }
    }
}
