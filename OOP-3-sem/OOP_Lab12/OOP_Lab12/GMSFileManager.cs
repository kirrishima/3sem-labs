using System.Globalization;
using System.IO;
using System.IO.Compression;

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

                logger.Info($"Listing for disk '{drive}' requested.");

                switch (drives.Count())
                {
                    case 0:
                        logger.Warning($"Disk '{driveName}' was not found.");
                        return;

                    case 1:
                        logger.Info($"Starting scan for '{driveName}'.");

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
                                    logger.Error($"Access denied while enumerating directories on '{driveName}': {ex.Message}");
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
                                    logger.Error($"Access denied while enumerating files on '{driveName}': {ex.Message}");
                                    fileStream.WriteLine($"Access denied while enumerating files: {ex.Message}");
                                }
                            }

                            logger.Info($"Scan for '{driveName}' completed. Results saved in '{outputFileName}'.");
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
                        logger.Warning($"'{driveName}' is ambiguous: more than one disk matches the name.");
                        break;
                }

                File.Copy(outputFileName, $"{outputFileName}.copy.txt", true);
                File.Move($"{outputFileName}.copy.txt", $"{outputFileName}.bak");
                logger.Info($"'{outputFileName} - copy' renamed to '{outputFileName}.bak'");

                File.Delete(outputFileName);
                logger.Info($"'{outputFileName}' deleted.");
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

        public static void CopyDirectory(string directoryName, string ext)
        {
            string targetBaseDir = "GMSFiles";
            logger.Info($"Starting copy process for folder {directoryName}. Will copy files with extension {ext}, to folder {targetBaseDir}");

            try
            {
                EnumerationOptions enumerationOptions = new()
                {
                    IgnoreInaccessible = true,
                    ReturnSpecialDirectories = false,
                    RecurseSubdirectories = true
                };

                Directory.CreateDirectory(targetBaseDir);

                foreach (var file in Directory.EnumerateFiles(directoryName, $"*.{ext}", enumerationOptions))
                {
                    try
                    {
                        var parentDir = Path.GetRelativePath(directoryName, file);
                        Directory.CreateDirectory(Path.Combine(targetBaseDir, parentDir));

                        var fileName = Path.GetFileName(file);

                        var path = Path.Combine(targetBaseDir, parentDir, fileName);
                        File.Copy(file, path, true);
                    }
                    catch (Exception ex)
                    {
                        logger.Error($"Failed to copy {file}: {ex.Message}");
                    }
                }

                var dest = Path.Combine("GMSInspect", targetBaseDir);

                if (!Directory.Exists(dest))
                {
                    Directory.Move(targetBaseDir, dest);
                    logger.Info($"moved {targetBaseDir} to GMSInspect");
                }
            }
            catch (Exception ex)
            {
                logger.Error("An error iccured");
                logger.PrintAllExeceptionsData(ex);
            }
        }

        public static void ZipGMSFiles(string source, string destination)
        {
            try
            {
                if (Directory.Exists(source))
                {
                    if (File.Exists(destination))
                    {
                        try
                        {
                            File.Delete(destination);
                        }
                        catch (Exception ex)
                        {
                            logger.Error($"failed to delete {destination} before zipping: {ex.Message}");
                        }
                    }
                    ZipFile.CreateFromDirectory(source, destination);
                }
            }
            catch (Exception ex)
            {
                logger.Error($"An error occured while zipping {source}: {ex.Message}");
            }
        }

        public static void LogFileSearch()
        {
            string logFilePath = "gmslogfile.txt";

            if (!File.Exists(logFilePath))
            {
                Console.WriteLine("Лог-файл не найден.");
                return;
            }

            var logLines = File.ReadAllLines(logFilePath);

            Console.WriteLine("Введите диапазон времени (формат: 'MM-dd HH:mm MM-dd HH:mm'), день ('MM-dd') или оставьте пустым: ");
            string timeFilter = Console.ReadLine();

            Console.WriteLine("Введите ключевое слово для поиска (или оставьте пустым): ");
            string keywordFilter = Console.ReadLine();

            DateTime? startTime = null;
            DateTime? endTime = null;

            if (!string.IsNullOrEmpty(timeFilter) && timeFilter.Contains(" "))
            {
                var times = timeFilter.Split(' ', StringSplitOptions.RemoveEmptyEntries);
                if (times.Length == 4 &&
                    DateTime.TryParseExact(times[0] + " " + times[1], "MM-dd HH:mm", CultureInfo.InvariantCulture, DateTimeStyles.None, out DateTime parsedStart) &&
                    DateTime.TryParseExact(times[2] + " " + times[3], "MM-dd HH:mm", CultureInfo.InvariantCulture, DateTimeStyles.None, out DateTime parsedEnd))
                {
                    startTime = parsedStart;
                    endTime = parsedEnd;
                }
                else
                {
                    Console.WriteLine("Ошибка: Неверный формат диапазона времени.");
                    return;
                }
            }
            else if (!string.IsNullOrEmpty(timeFilter))
            {
                if (DateTime.TryParseExact(timeFilter, "MM-dd", CultureInfo.InvariantCulture, DateTimeStyles.None, out DateTime parsedDay))
                {
                    startTime = parsedDay;
                    endTime = DateTime.MaxValue;
                }
                else
                {
                    Console.WriteLine("Ошибка: Неверный формат дня.");
                    return;
                }
            }

            var filteredLines = logLines.Where(line =>
            {
                if (string.IsNullOrEmpty(keywordFilter) && startTime == endTime)
                {
                    return true;
                }
                if (line.Length < 19 || !DateTime.TryParseExact(line.Substring(0, 19), "MM-dd HH:mm:ss:ffff",
                    CultureInfo.InvariantCulture, DateTimeStyles.None, out DateTime logTime))
                {
                    return false;
                }

                bool matchesTime = (!startTime.HasValue || logTime >= startTime) && (!endTime.HasValue || logTime <= endTime);

                bool matchesKeyword = line.Contains(keywordFilter, StringComparison.OrdinalIgnoreCase);

                return matchesTime && matchesKeyword;
            }).ToList();

            Console.WriteLine("Найденные записи:");
            foreach (var line in filteredLines)
            {
                Console.WriteLine(line);
            }

            Console.WriteLine($"Общее количество найденных записей: {filteredLines.Count}");

            string currentTimePrefix = DateTime.Now.ToString("MM-dd HH");
            var currentHourLines = logLines
                .Where(line => line.StartsWith(currentTimePrefix))
                .ToList();

            File.WriteAllLines($"{logFilePath}.last_hour.txt", currentHourLines);

            Console.WriteLine("Записи за текущий час сохранены.");
        }
    }
}
