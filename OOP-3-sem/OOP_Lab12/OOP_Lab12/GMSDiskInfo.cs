namespace OOP_Lab12
{
    internal static class GMSDiskInfo
    {
        public static void PrintFreeSpace()
        {
            var drives = DriveInfo.GetDrives();

            Console.WriteLine($"Drive name\tTotal space\tAvailable space\n");
            foreach (var drive in drives)
            {
                Console.WriteLine($"{drive.Name.PadRight(10)}\t{ConvertBytes(drive.TotalSize).PadRight(11)}\t{ConvertBytes(drive.AvailableFreeSpace).PadRight(15)}");
            }
        }

        private static string ConvertBytes(double bytes)
        {
            string[] units = { "bytes", "KB", "MB", "GB", "TB" };
            int unitIndex = 0;

            while (bytes > 1024 && unitIndex < units.Length - 1)
            {
                if ((int)(bytes / 1024) == 0) break;
                bytes /= 1024;
                unitIndex++;
            }

            return $"{bytes:N2} {units[unitIndex]}";
        }

        public static void PrintFilesystem()
        {
            var drives = DriveInfo.GetDrives();

            Console.WriteLine($"Drive name\tFile system\n");
            foreach (var drive in drives)
            {
                Console.WriteLine($"{drive.Name.PadRight(10)}\t{drive.DriveFormat.PadRight(11)}");
            }
        }

        public static void PrintFullInfo()
        {
            var drives = DriveInfo.GetDrives();

            Console.WriteLine($"Drive name\tLabel\tTotal space\tAvailable space\n");
            foreach (var drive in drives)
            {
                Console.WriteLine($"{drive.Name.PadRight(10)}\t{drive.VolumeLabel.PadRight(5)}\t{ConvertBytes(drive.TotalSize).PadRight(11)}\t{ConvertBytes(drive.AvailableFreeSpace).PadRight(15)}");
            }
        }
    }
}
