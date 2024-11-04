using System;
using System.Linq;
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
                new Airline("New York, USA", 102, "Boeing 747", new TimeOnly(14, 15, 0), [DayOfWeek.Wednesday, DayOfWeek.Friday]),
                new Airline("Tokyo, Japan", 103, "Airbus A380", new TimeOnly(8, 45, 0), [DayOfWeek.Monday, DayOfWeek.Thursday]),
                new Airline("Paris, France", 104, "Boeing 787", new TimeOnly(16, 0, 0), [DayOfWeek.Sunday, DayOfWeek.Thursday]),
                new Airline("Berlin, Germany", 105, "Embraer E190", new TimeOnly(12, 30, 0), [DayOfWeek.Monday, DayOfWeek.Friday]),
                new Airline("Sydney, Australia", 106, "Boeing 777", new TimeOnly(18, 0, 0), [DayOfWeek.Wednesday, DayOfWeek.Saturday]),
                new Airline("Moscow, Russia", 107, "Sukhoi Superjet 100", new TimeOnly(13, 15, 0), [DayOfWeek.Tuesday, DayOfWeek.Friday]),
                new Airline("Dubai, UAE", 108, "Airbus A350", new TimeOnly(20, 45, 0), [DayOfWeek.Thursday]),
                new Airline("Toronto, Canada", 109, "Boeing 737", new TimeOnly(9, 10, 0), [DayOfWeek.Monday, DayOfWeek.Friday]),
                new Airline("Toronto, Canada", 110, "Boeing 787", new TimeOnly(7, 0, 0), [DayOfWeek.Wednesday, DayOfWeek.Saturday])
            ];

            PrintElements(airlines);
            // 3

            var dest = airlines.Where((a) => a.Destination.StartsWith("Toronto"));

            PrintElements(dest);

            Console.WriteLine("\nКоличество рейсов для заданного дня недели (Monday)");
            var c = from line in airlines where line.DaysOfWeeks.Contains(DayOfWeek.Monday) orderby line.DepartureTime select line;
            Console.WriteLine(c.Count());

            Console.WriteLine("\nРейс который вылетает в понедельник раньше всех");
            Console.WriteLine(c.Take(1).First());
            Console.WriteLine("\nРейс который вылетает в среду или пятницу  позже всех");

            var cc = from line in airlines
                     where line.DaysOfWeeks.Contains(DayOfWeek.Wednesday) || line.DaysOfWeeks.Contains(DayOfWeek.Friday)
                     orderby line.DepartureTime descending
                     select line;
            Console.WriteLine(cc.First());

            Console.WriteLine("\nСписок рейсов, упорядоченных по времени вылета");
            var ccc = from line in airlines orderby line.DepartureTime.Ticks, line.ID select line;
            PrintElements(ccc);

            // 4

            var myQuery = from line in airlines
                          where line.PlaneType.Contains("Boeing")
                          orderby line.FlightNumber
                          group line by string.Join(", ", line.DaysOfWeeks);

            Console.WriteLine("Мой мега пупер классный запрос:");
            foreach (var item in myQuery.TakeWhile(g => g.Key != "Aboba"))
            {
                // Выводим дни недели, по которым сгруппирован рейс
                Console.WriteLine($"Дни отправки: {item.Key}");
                foreach (var line in item)
                {
                    Console.WriteLine(line);
                }
                Console.WriteLine();
            }

            // 5

            var first = airlines.Take(airlines.Count / 2);
            var second = airlines.Skip(airlines.Count / 2);

            PrintElements(first);
            PrintElements(second);


            var res = from line in first
                      join otherLine in second on new
                      {
                          x = line.DaysOfWeeks.FirstOrDefault(),
                          y = line.DaysOfWeeks.LastOrDefault()
                      } equals new
                      {
                          x = otherLine.DaysOfWeeks.FirstOrDefault(),
                          y = otherLine.DaysOfWeeks.LastOrDefault()
                      }
                      select $"1st: {line}\n2nd: {otherLine}\n";

            PrintElements(res);
        }
    }
}