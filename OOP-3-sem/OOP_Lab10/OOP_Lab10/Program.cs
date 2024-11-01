using System;
using OOP_Lab02;

namespace OOP_Lab10
{
    internal class Program
    {
        private static void Main(string[] args)
        {
            void PrintElements<T>(IEnumerable<T> values)
            {
                Console.WriteLine();
                foreach (var item in values)
                {
                    Console.WriteLine(item);
                }
                Console.WriteLine();
            }

            // 1 задание
            string[] months = { "December", "January", "February", "March", "April", "May", "September", "October", "November", "June", "July", "August" };

            const int n = 7;

            var a = from m in months where m.Length == n select m;

            PrintElements(a);

            a = months.Take(3).Concat(months.Skip(9));
            PrintElements(a);

            a = from m in months orderby m select m;
            PrintElements(a);

            a = from m in months where m.Contains('u') && m.Length > 3 select m;
            PrintElements(a);

            // 2

            List<Airline> airlines = [
                new Airline("London, UK", 101, "Airbus A320", new TimeOnly(10, 30, 0), [DayOfWeek.Tuesday, DayOfWeek.Friday]),
                new Airline("New York, USA", 102, "Boeing 747", new TimeOnly(14, 15, 0), [DayOfWeek.Wednesday, DayOfWeek.Saturday]),
                new Airline("Tokyo, Japan", 103, "Airbus A380", new TimeOnly(8, 45, 0), [DayOfWeek.Monday, DayOfWeek.Thursday]),
                new Airline("Paris, France", 104, "Boeing 787", new TimeOnly(16, 0, 0), [DayOfWeek.Sunday, DayOfWeek.Thursday]),
                new Airline("Berlin, Germany", 105, "Embraer E190", new TimeOnly(12, 30, 0), [DayOfWeek.Monday, DayOfWeek.Friday]),
                new Airline("Sydney, Australia", 106, "Boeing 777", new TimeOnly(18, 0, 0), [DayOfWeek.Wednesday, DayOfWeek.Saturday]),
                new Airline("Moscow, Russia", 107, "Sukhoi Superjet 100", new TimeOnly(13, 15, 0), [DayOfWeek.Tuesday, DayOfWeek.Friday]),
                new Airline("Dubai, UAE", 108, "Airbus A350", new TimeOnly(20, 45, 0), [DayOfWeek.Thursday]),
                new Airline("Toronto, Canada", 109, "Boeing 737", new TimeOnly(9, 10, 0), [DayOfWeek.Monday, DayOfWeek.Friday]),
                new Airline("Toronto, Canada", 110, "Boeing 787", new TimeOnly(7, 0, 0), [DayOfWeek.Wednesday, DayOfWeek.Saturday])
            ];

            // 3

            var dest = airlines.Where((a) => a.Destination.StartsWith("Toronto"));

            PrintElements(dest);

            int c = (from line in airlines where line.DaysOfWeeks.Contains(DayOfWeek.Monday) select line).Count();
            Console.WriteLine(c);
        }
    }
}