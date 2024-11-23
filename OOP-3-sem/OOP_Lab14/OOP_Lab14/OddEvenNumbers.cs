namespace OOP_Lab14
{
    internal class OddEvenNumbers
    {
        private static readonly object _lockObject = new();
        private static bool _evenTurn = true;
        private static StreamWriter? _writer;

        public static void Showcase()
        {
            _writer = new StreamWriter("output.txt", false);

            Thread evenThread = new Thread(() => EvenNumbers(100))
            {
                Priority = ThreadPriority.AboveNormal
            };

            Thread oddThread = new Thread(() => OddNumbers(100))
            {
                Priority = ThreadPriority.BelowNormal
            };

            evenThread.Start();
            oddThread.Start();

            evenThread.Join();
            oddThread.Join();

            _writer.Close();
        }

        public static void EvenNumbers(int n)
        {
            for (int i = 0; i <= n; i += 2)
            {
                lock (_lockObject)
                {
                    while (!_evenTurn)
                    {
                        Monitor.Wait(_lockObject);
                    }

                    Console.Write($"{i} ");
                    _writer?.Write($"{i} ");
                    _evenTurn = false;
                    Monitor.Pulse(_lockObject);
                }

                Thread.Sleep(100);
            }
        }

        public static void OddNumbers(int n)
        {
            for (int i = 1; i <= n; i += 2)
            {
                lock (_lockObject)
                {
                    while (_evenTurn)
                    {
                        Monitor.Wait(_lockObject);
                    }

                    Console.Write($"{i} ");
                    _writer?.Write($"{i} ");
                    _evenTurn = true;
                    Monitor.Pulse(_lockObject);
                }

                Thread.Sleep(50);
            }
        }
    }
}
