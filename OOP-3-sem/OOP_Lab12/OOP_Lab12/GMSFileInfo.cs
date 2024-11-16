using Microsoft.VisualBasic;

namespace OOP_Lab12
{
    internal static class GMSFileInfo
    {
        public static void PrintFullPath(string filename)
        {
            Console.WriteLine(Path.GetFullPath(filename));
        }

        public static void PrintFileInfo(string filename)
        {
            FileInfo fileInfo = new FileInfo(filename);
            Console.WriteLine($"Name: {fileInfo.Name}. Extension: {fileInfo.Extension}. Size: {fileInfo.Length}");
        }
    }
}
