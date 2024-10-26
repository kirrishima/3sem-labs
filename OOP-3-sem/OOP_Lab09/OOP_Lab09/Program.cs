using System;

namespace OOP_Lab09
{
    class Program
    {
        static void Main(string[] args)
        {
            void Print<K, V>(IDictionary<K, V> book, string s)
            {
                Console.WriteLine($"\n{s}");
                foreach (var item in (IDictionary<K, V>)book)
                {
                    Console.WriteLine($"Key: {item.Key}, Value: {item.Value}");
                }
            }

            var books = new Books<int, char>();
            books.Add(1, 'A');
            books.Add(2, 'B');
            books.Add(3, 'C');
            books.Add(4, 'D');
            books.Add(5, 'E');

            Print(books, "Коллекция Books:");

            books.Remove(2);
            books.Remove(3);

            Print(books, "После удаления:");

            books.Add(6, 'F');
            books[7] = 'G';

            Print(books, "После добавления:");

            var dictionary = new Dictionary<int, char>();

            foreach (var item in books)
            {
                dictionary.Add(item.Key, item.Value);
            }

            Print(dictionary, "Коллекция Dictionary:");
        }
    }
}