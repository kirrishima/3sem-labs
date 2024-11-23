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
                Console.WriteLine($"{drive.Name,-10}\t{ConvertBytes(drive.TotalSize),-11}\t{ConvertBytes(drive.AvailableFreeSpace),-15}");
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
                Console.WriteLine($"{drive.Name,-10}\t{drive.DriveFormat,-11}");
            }
        }

        public static void PrintFullInfo()
        {
            var drives = DriveInfo.GetDrives();

            Console.WriteLine($"{"Drive name",-16}{"Label",-15}{"Total space",-17}Available space\n");


            foreach (var drive in drives)
            {
                Console.WriteLine($"{drive.Name,-16}{drive.VolumeLabel,-15}{ConvertBytes(drive.TotalSize),-17}{ConvertBytes(drive.AvailableFreeSpace)}");
            }
        }
    }
}
