using System;
using System.IO;
using System.Linq;

class HexEditor
{
    static void Main()
    {
        Console.WriteLine("=== Hex Editor ===");
        Console.Write("Enter file path: ");
        string filePath = Console.ReadLine();

        byte[] fileBytes;

        // Если файл не существует, создаём его
        if (!File.Exists(filePath))
        {
            Console.WriteLine("File not found. Creating a new file filled with zeros...");
            fileBytes = new byte[160]; // 10x16 байт
            File.WriteAllBytes(filePath, fileBytes);
        }
        else
        {
            fileBytes = File.ReadAllBytes(filePath);

            // Если файл пустой, заполняем нулями до 10x16 байт
            if (fileBytes.Length == 0)
            {
                Console.WriteLine("File is empty. Initializing with zeros (10x16 grid)...");
                fileBytes = new byte[160];
                File.WriteAllBytes(filePath, fileBytes);
            }
        }

        bool running = true;

        while (running)
        {
            Console.Clear();
            PrintHex(fileBytes);

            Console.WriteLine("\nOptions:");
            Console.WriteLine("1. Edit byte");
            Console.WriteLine("2. Save and exit");
            Console.WriteLine("3. Exit without saving");
            Console.Write("Choose an option: ");

            switch (Console.ReadLine())
            {
                case "1":
                    EditByte(fileBytes);
                    break;
                case "2":
                    File.WriteAllBytes(filePath, fileBytes);
                    running = false;
                    break;
                case "3":
                    running = false;
                    break;
                default:
                    Console.WriteLine("Invalid option, press any key to try again.");
                    Console.ReadKey();
                    break;
            }
        }

        Console.WriteLine("Goodbye!");
    }

    static void PrintHex(byte[] data)
    {
        const int bytesPerRow = 16;

        for (int i = 0; i < data.Length; i += bytesPerRow)
        {
            Console.Write($"{i:X8}: ");

            var rowBytes = data.Skip(i).Take(bytesPerRow).ToArray();
            foreach (var b in rowBytes)
                Console.Write($"{b:X2} ");

            Console.Write("| ");
            foreach (var b in rowBytes)
                Console.Write(char.IsControl((char)b) ? '.' : (char)b);

            Console.WriteLine();
        }
    }

    static void EditByte(byte[] data)
    {
        Console.Write("Enter byte offset (hex): ");
        if (!int.TryParse(Console.ReadLine(), System.Globalization.NumberStyles.HexNumber, null, out int offset) || offset < 0 || offset >= data.Length)
        {
            Console.WriteLine("Invalid offset, press any key to return to menu.");
            Console.ReadKey();
            return;
        }

        Console.Write($"Current value at {offset:X}: {data[offset]:X2}. Enter new value (hex): ");
        if (!byte.TryParse(Console.ReadLine(), System.Globalization.NumberStyles.HexNumber, null, out byte newValue))
        {
            Console.WriteLine("Invalid value, press any key to return to menu.");
            Console.ReadKey();
            return;
        }

        data[offset] = newValue;
        Console.WriteLine("Byte updated. Press any key to return to menu.");
        Console.ReadKey();
    }
}
