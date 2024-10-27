using System;
using System.Collections.ObjectModel;
using System.Collections.Specialized;

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

            Print(books, "��������� Books:");

            books.Remove(2);
            books.Remove(3);

            Print(books, "����� ��������:");

            books.Add(6, 'F');
            books[7] = 'G';

            Print(books, "����� ����������:");

            var dictionary = new Dictionary<int, char>();

            foreach (var item in books)
            {
                dictionary.Add(item.Key, item.Value);
            }

            Print(dictionary, "��������� Dictionary:");

            ObservableCollection<Books<int, char>> observableCollection = [];

            observableCollection.CollectionChanged += OnChange;
            observableCollection.Add(books);
            observableCollection.RemoveAt(0);
        }

        private static void OnChange(object? sender, NotifyCollectionChangedEventArgs e)
        {
            switch (e.Action)
            {
                case NotifyCollectionChangedAction.Add:
                    Console.WriteLine("\n�������� ������� � ��������� MyCollection");
                    break;
                case NotifyCollectionChangedAction.Remove:
                    Console.WriteLine("\n����� ������� � ��������� MyCollection");
                    break;
            }
        }
    }
}