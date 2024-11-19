using System.Security.AccessControl;

namespace OOP_Lab12
{
    internal static class GMSDirInfo
    {
        public static void PrintFilesCount(string directory)
        {
            if (Directory.Exists(directory))
            {
                Console.WriteLine($"{Directory.GetFiles(directory, "*", SearchOption.AllDirectories).Length} files.");
            }
            else
            {
                Console.WriteLine($"{directory} does not exists.");
            }
        }

        public static void PrintCreationDate(string directory)
        {
            if (Directory.Exists(directory))
            {
                Console.WriteLine($"{Directory.GetCreationTime(directory)}");
            }
            else
            {
                Console.WriteLine($"{directory} does not exists.");
            }
        }

        public static void PrintSubDirsCount(string directory)
        {
            if (Directory.Exists(directory))
            {
                Console.WriteLine($"{Directory.GetDirectories(directory).Length} sub dirs.");
            }
            else
            {
                Console.WriteLine($"{directory} does not exists.");
            }
        }

        public static void PrintParentDirs(string directory)
        {
            if (Directory.Exists(directory))
            {
                Console.WriteLine($"Parent dirs for {directory}:");
                var parentDir = Directory.GetParent(directory);
                while (parentDir?.Exists == true)
                {
                    Console.WriteLine($"\t{parentDir.Name}");
                    parentDir = parentDir.Parent;
                }
            }
            else
            {
                Console.WriteLine($"{directory} does not exists.");
            }
        }
    }
}
