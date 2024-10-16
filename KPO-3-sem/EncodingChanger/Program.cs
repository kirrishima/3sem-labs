using System.CommandLine;
using System.CommandLine.Builder;
using System.CommandLine.Parsing;
using System.Text;

class Program
{
    static Task<int> Main(string[] args)
    {
        Encoding.RegisterProvider(CodePagesEncodingProvider.Instance);

        var directoryOption = new Option<string>(
            name: "--directory",
            description: "Path to the directory where files will be processed.",
            getDefaultValue: () => Directory.GetCurrentDirectory()
        );
        directoryOption.AddAlias("-d");

        var encodingOption = new Option<string>(
            name: "--encoding",
            description: "Target encoding for files (e.g., UTF8, UTF8-BOM).",
            getDefaultValue: () => "UTF8-BOM"
        );
        encodingOption.AddAlias("-e");

        var recursiveOption = new Option<bool>(
            name: "--recursive",
            description: "Whether to process files in subdirectories.",
            getDefaultValue: () => false
        );
        recursiveOption.AddAlias("-r");

        var rootCommand = new RootCommand("File Encoding Converter")
        {
            directoryOption,
            encodingOption,
            recursiveOption
        };

        rootCommand.SetHandler((string directory, string encoding, bool recursive) =>
        {
            Encoding targetEncoding = encoding.ToUpper() switch
            {
                "UTF8" => new UTF8Encoding(false),
                "UTF8-BOM" => new UTF8Encoding(true),
                "WINDOWS-1251" => Encoding.GetEncoding("windows-1251"),
                _ => throw new ArgumentException("Unsupported encoding type.")
            };

            ProcessFiles(directory, targetEncoding, recursive);
        }, directoryOption, encodingOption, recursiveOption);

        var parser = new CommandLineBuilder(rootCommand)
            .UseDefaults()
            .Build();

        return parser.InvokeAsync(args);
    }

    static void ProcessFiles(string directoryPath, Encoding targetEncoding, bool includeSubdirectories)
    {
        SearchOption searchOption = includeSubdirectories ? SearchOption.AllDirectories : SearchOption.TopDirectoryOnly;

        string[] files = Directory.GetFiles(directoryPath, "*.*", searchOption)
                                   .Where(f => f.EndsWith(".cpp") || f.EndsWith(".h"))
                                   .ToArray();

        foreach (string file in files)
        {
            ChangeFileEncoding(file, targetEncoding);
        }
    }

    static void ChangeFileEncoding(string filePath, Encoding targetEncoding)
    {
        try
        {
            Encoding sourceEncoding = DetectFileEncoding(filePath);

            if (targetEncoding.Equals(sourceEncoding))
            {
                Console.WriteLine($"Кодировка файла '{filePath}' уже {targetEncoding.EncodingName}");
                return;
            }

            string fileContent;
            using (StreamReader reader = new StreamReader(filePath, sourceEncoding))
            {
                fileContent = reader.ReadToEnd();
            }

            using (StreamWriter writer = new StreamWriter(filePath, false, targetEncoding))
            {
                writer.Write(fileContent);
            }

            Console.WriteLine($"Кодировка файла '{filePath}' успешно изменена.");
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Ошибка при изменении кодировки файла '{filePath}': {ex.Message}");
        }
    }

    static Encoding DetectFileEncoding(string filePath)
    {
        var bom = new byte[4];
        using (var file = new FileStream(filePath, FileMode.Open, FileAccess.Read))
        {
            file.Read(bom, 0, 4);
        }

        if (bom[0] == 0x2b && bom[1] == 0x2f && bom[2] == 0x76) return Encoding.UTF7;
        if (bom[0] == 0xef && bom[1] == 0xbb && bom[2] == 0xbf) return new UTF8Encoding(true);
        if (bom[0] == 0xff && bom[1] == 0xfe) return Encoding.Unicode;
        if (bom[0] == 0xfe && bom[1] == 0xff) return Encoding.BigEndianUnicode;
        if (bom[0] == 0 && bom[1] == 0 && bom[2] == 0xfe && bom[3] == 0xff) return Encoding.UTF32;

        return Encoding.GetEncoding("windows-1251");
    }
}
