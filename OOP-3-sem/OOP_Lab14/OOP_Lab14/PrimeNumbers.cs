using System.Text;

namespace OOP_Lab14
{
    internal class PrimeNumbers
    {
        private static Thread? _primeThread;
        private static bool _pauseRequested = false;

        public static void StartShowcase()
        {
            _primeThread = new Thread(() => PrintN(10000))
            {
                Name = "PrimeCalculationThread",
                Priority = ThreadPriority.Normal
            };

            Console.WriteLine("Управление потоком:");
            Console.WriteLine("[S] - Запустить поток");
            Console.WriteLine("[P] - Приостановить поток");
            Console.WriteLine("[R] - Возобновить поток");
            Console.WriteLine("[T] - Остановить поток");
            Console.WriteLine("[E] - Выход");

            bool exit = false;
            while (!exit)
            {
                char command = Console.ReadKey(true).KeyChar;
                switch (char.ToUpper(command))
                {
                    case 'S':
                        if (_primeThread.ThreadState == System.Threading.ThreadState.Unstarted)
                        {
                            Console.WriteLine("Запуск потока...");
                            _primeThread.Start();
                        }
                        else
                        {
                            Console.WriteLine("Поток уже запущен.");
                        }
                        break;

                    case 'P':
                        Console.WriteLine("Приостановка потока...");
                        _pauseRequested = true;
                        break;

                    case 'R':
                        Console.WriteLine("Возобновление потока...");
                        _pauseRequested = false;
                        break;

                    case 'T':
                        Console.WriteLine("Остановка потока...");
                        _primeThread.Interrupt();
                        _primeThread.Join();
                        Console.WriteLine("Поток завершён.");
                        exit = true;
                        break;

                    case 'E':
                        exit = true;
                        break;

                    default:
                        Console.WriteLine("Неверная команда. Попробуйте снова.");
                        break;

                }

                if (_primeThread != null)
                {
                    try
                    {
                        Console.WriteLine($"\nСтатус потока: {_primeThread.ThreadState}");
                        Console.WriteLine($"Имя: {_primeThread.Name}");
                        Console.WriteLine($"Приоритет: {_primeThread.Priority}");
                        Console.WriteLine($"Идентификатор: {_primeThread.ManagedThreadId}\n");
                    }
                    catch { }
                }

                Console.WriteLine("\nУправление потоком:");
                Console.WriteLine("[S] - Запустить поток");
                Console.WriteLine("[P] - Приостановить поток");
                Console.WriteLine("[R] - Возобновить поток");
                Console.WriteLine("[T] - Остановить поток");
                Console.WriteLine("[E] - Выход");
            }
        }
        public static void PrintN(int n)
        {
            try
            {
                int count = 0;
                using (StreamWriter fs = new("numbers.txt", append: false, encoding: Encoding.UTF8))
                {
                    for (int i = 1; i <= n; i++)
                    {
                        Thread.Sleep(50);
                        if (_pauseRequested)
                        {
                            fs.Flush();
                            Thread.Sleep(100);
                            continue;
                        }
                        if (IsPrime(i))
                        {
                            string str = $"{i,-5} ";
                            if (count == 20)
                            {
                                str = "\n" + str;
                                count = 0;
                            }

                            Console.Write(str);
                            fs.Write(str);

                            count++;
                        }
                    }
                }
            }
            catch (ThreadInterruptedException)
            {
                Console.WriteLine("Поток был прерван из вне.");
            }
        }

        public static bool IsPrime(int number)
        {
            if (number == 2)
                return true;
            if (number % 2 == 0)
                return false;

            int boundary = (int)Math.Sqrt(number);
            for (int i = 3; i <= boundary; i += 2)
            {
                if (number % i == 0)
                    return false;
            }
            return true;
        }
    }


}
