using System.Reflection;

namespace OOP_Lab12
{
    class Program
    {
        static void Main(string[] args)
        {
            string currentPath = Directory.GetCurrentDirectory();
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

            string baseOOPfolder = "..\\..\\..\\..\\..\\";

            Console.WriteLine();
            string lecturesPath = Path.GetFullPath(Path.Combine(currentPath, $"{baseOOPfolder}\\Лекции"));
            GMSDirInfo.PrintFilesCount(lecturesPath);
            GMSDirInfo.PrintCreationDate(lecturesPath);
            GMSDirInfo.PrintSubDirsCount(lecturesPath);
            GMSDirInfo.PrintParentDirs(lecturesPath);

            Console.WriteLine();
            DriveInfo driveInfo = new DriveInfo("c");
            Console.WriteLine(driveInfo.Name);

            var lab4Path = Path.GetFullPath(Path.Combine(currentPath, $"{baseOOPfolder}oop_lab04"));

            GMSFileManager.ListDrive("e");
            GMSFileManager.CopyDirectory(lab4Path, "cs");

            GMSFileManager.ZipGMSFiles(Path.Combine(currentPath, "GMSFiles"), "aboba.zip");
            GMSFileManager.LogFileSearch();
        }
    }
}