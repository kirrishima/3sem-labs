using System;

namespace OOP_Lab09
{
    class Program
    {
        static void Main(string[] args)
        {
            var books = new Books<int, char>();
            books.Add(1, 'A');
            books.Add(2, 'B');
            books.Add(3, 'C');
            books.Add(4, 'D');
            books.Add(5, 'E');

            Console.WriteLine("��������� Books:");
            foreach (var item in books)
            {
                Console.WriteLine($"Key: {item.Key}, Value: {item.Value}");
            }

            books.Remove(2);
            books.Remove(3);

            Console.WriteLine("\n����� ��������:");
            foreach (var item in books)
            {
                Console.WriteLine($"Key: {item.Key}, Value: {item.Value}");
            }

            books.Add(6, 'F');
            books[7] = 'G';

            Console.WriteLine("\n����� ����������:");
            foreach (var item in books)
            {
                Console.WriteLine($"Key: {item.Key}, Value: {item.Value}");
            }

            var dictionary = new Dictionary<int, char>();

            foreach (var item in books)
            {
                dictionary.Add(item.Key, item.Value);
            }

            Console.WriteLine("\n��������� Dictionary:");
            foreach (var item in dictionary)
            {
                Console.WriteLine($"Key: {item.Key}, Value: {item.Value}");
            }

            char searchValue = 'D';
            bool found = false;
            foreach (var kvp in dictionary)
            {
                if (kvp.Value == searchValue)
                {
                    Console.WriteLine($"\n�������� '{searchValue}' ������� � ������ {kvp.Key}.");
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                Console.WriteLine($"\n�������� '{searchValue}' �� �������.");
            }
        }
    }
}