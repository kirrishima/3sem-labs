using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OOP_Lab15
{
    internal class Warehouse
    {
        private BlockingCollection<string> _goods = new(new ConcurrentQueue<string>());
        private static object _locker = new object();
        private static object _consoleLock = new object();
        Random random = new Random();

        protected static void PrintColored(ConsoleColor color, string text)
        {
            lock (_consoleLock)
            {
                var originalColor = Console.ForegroundColor;
                Console.ForegroundColor = color;
                Console.WriteLine(text);
                Console.ForegroundColor = originalColor;
            }
        }

        private async Task Peek()
        {
            string[] names = { "Олег", "Жора", "Аркадий Паровозов", "Семен Семеныч",
                "Борис Бритва", "Петрович", "Геннадий Горыныч", "Евгений Онегин", "Саня 228", "Василий Теркин" };

            while (true)
            {
                PeekGood(names[random.Next(0, 9)]);
                await Task.Delay(1000);
            }
        }

        public void AddGood(string good)
        {
            _goods.Add(good);
        }

        public void PeekGood(string person)
        {
            if (_goods.TryTake(out string? good))
            {
                PrintColored(ConsoleColor.Green, $"{person} успел урвать {good}! Повезло повезло.");

                var remainingGoods = _goods.ToArray();

                Console.WriteLine("Оставшиеся товары:");
                foreach (var item in remainingGoods)
                {
                    PrintColored(ConsoleColor.Yellow, item);
                }
            }
            else
            {
                Console.WriteLine($"{person} не смог ничего купить и свалил!");
            }
        }

        public async Task Start()
        {
            string[] products = { "Кружка-грелка", "Носки-самонаводящиеся", "Баян 3000", "Котоматрица", "Шапка-невидимка" };

            Peek();

            for (int i = 0; i < 5; i++)
            {
                await Task.Run(async () =>
                {
                    await Task.Delay(random.Next(300, 2000));
                    _goods.Add(products[i]);
                });
            }

            while (_goods.Count != 0) ;
        }
    }
}
