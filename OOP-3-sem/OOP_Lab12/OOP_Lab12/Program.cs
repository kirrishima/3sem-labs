using System;
using System.Reflection;

namespace OOP_Lab12
{
    class Program
    {
        static void Main(string[] args)
        {

            // 2 
            GMSDiskInfo.PrintFreeSpace();

            Console.WriteLine();
            GMSDiskInfo.PrintFilesystem();

            Console.WriteLine();
            GMSDiskInfo.PrintFullInfo();

            Console.WriteLine();
            GMSFileInfo.PrintFileInfo($"{Assembly.GetCallingAssembly().GetName().Name}.dll");
        }
    }
}