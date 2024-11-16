using System.IO;

namespace OOP_Lab12
{
    internal static class GMSFileManager
    {
        private static Logger logger;
        private static bool initialized = false;
        private static readonly string logfile = "gmslogfile.txt";

        static GMSFileManager()
        {
            logger = new Logger(msg =>
            {
                using (StreamWriter fileStream = new(logfile, append: true))
                {
                    fileStream.Write(msg);
                }
            });

            logger.LoggingAction.Invoke("\n\n\n");
        }

        /// <summary>
        /// Только буква диска
        /// </summary>
        /// <param name="drive"></param>
        public static void ListDrive(string drive)
        {
            string outputFileName = Path.Combine("GMSInspect", "gmsdirinfo.txt");

            try
            {
                Directory.CreateDirectory("GMSInspect");

                logger.Info("Created  directory.");

                DriveInfo driveInfo = new DriveInfo(drive);
                string driveName = driveInfo.Name;

                var drives = DriveInfo.GetDrives().Where(d => d.Name.Equals(driveName, StringComparison.OrdinalIgnoreCase));

                logger.Info($"Listing for disk {drive} requested.");

                switch (drives.Count())
                {
                    case 0:
                        logger.Warning($"Disk {driveName} was not found.");
                        return;

                    case 1:
                        logger.Info($"Starting scan for {driveName}.");

                        if (File.Exists(outputFileName))
                        {
                            logger.Warning($"Output file '{outputFileName}' already exists. It will be overwritten.");
                        }

                        try
                        {

                            using (StreamWriter fileStream = new(outputFileName, append: false))
                            {
                                EnumerationOptions enumerationOptions = new()
                                {
                                    IgnoreInaccessible = true,
                                    ReturnSpecialDirectories = false,
                                    RecurseSubdirectories = true
                                };

                                fileStream.WriteLine("Directories:");
                                try
                                {
                                    foreach (var dir in Directory.EnumerateDirectories(driveName, "*", enumerationOptions))
                                    {
                                        fileStream.WriteLine(dir);
                                    }
                                }
                                catch (UnauthorizedAccessException ex)
                                {
                                    logger.Error($"Access denied while enumerating directories on {driveName}: {ex.Message}");
                                    fileStream.WriteLine($"[ERROR] Access denied while enumerating directories: {ex.Message}");
                                }

                                fileStream.WriteLine("\nFiles:");
                                try
                                {
                                    foreach (var file in Directory.EnumerateFiles(driveName, "*", enumerationOptions))
                                    {
                                        fileStream.WriteLine(file);
                                    }
                                }
                                catch (UnauthorizedAccessException ex)
                                {
                                    logger.Error($"Access denied while enumerating files on {driveName}: {ex.Message}");
                                    fileStream.WriteLine($"[ERROR] Access denied while enumerating files: {ex.Message}");
                                }
                            }

                            logger.Info($"Scan for {driveName} completed. Results saved in '{outputFileName}'.");
                        }
                        catch (IOException ex)
                        {
                            logger.Error($"Failed to write to output file '{outputFileName}': {ex.Message}");
                        }
                        catch (Exception ex)
                        {
                            logger.Error($"Unexpected error during scan for {driveName}: {ex.Message}");
                        }

                        break;

                    default:
                        logger.Warning($"{driveName} is ambiguous: more than one disk matches the name.");
                        break;
                }

                File.Copy(outputFileName, $"{outputFileName}.bak", true);
                logger.Info($"{outputFileName} renamed to {outputFileName}.bak");

                File.Delete(outputFileName);
                logger.Info($"{outputFileName} deleted.");
            }
            catch (ArgumentException ex)
            {
                logger.Error($"Invalid drive specified: {ex.Message}");
            }
            catch (Exception ex)
            {
                logger.Error($"Unexpected error occurred: {ex.Message}");
            }
        }
    }
}
