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
            GMSFileInfo.PrintFullPath($"{Assembly.GetCallingAssembly().GetName().Name}.dll");
            GMSFileInfo.PrintDates($"{Assembly.GetCallingAssembly().GetName().Name}.dll");

            Console.WriteLine();
            GMSDirInfo.PrintFilesCount("E:\\3 sem\\OOP-3-sem\\Лекции");
            GMSDirInfo.PrintCreationDate("E:\\3 sem\\OOP-3-sem\\Лекции");
            GMSDirInfo.PrintSubDirsCount("E:\\3 sem\\OOP-3-sem\\Лекции");
            GMSDirInfo.PrintParentDirs("E:\\3 sem\\OOP-3-sem\\Лекции");

            Console.WriteLine();
            DriveInfo driveInfo = new DriveInfo("c");
            Console.WriteLine(driveInfo.Name);

            //GMSFileManager.ListDrive("c");
            GMSFileManager.CopyDirectory("C:\\Users\\Force\\Documents\\University\\3sem\\OOP-3-sem\\oop_lab04", "cs");
        }
    }
}