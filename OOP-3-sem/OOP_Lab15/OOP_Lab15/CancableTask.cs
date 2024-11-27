using System.Diagnostics;

namespace OOP_Lab15
{
    internal static class CancableTask
    {
        public static void Task()
        {
            Stopwatch stopwatch = new Stopwatch();
            int[] ints = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 52, 53, 54, 55, 56, 67, 78, 12, 432, 8, 8 };
            SwampSort.Shuffle(ints);

            foreach (int i in ints)
            {
                Console.Write($"{i} ");
            }
            Console.WriteLine();

            CancellationTokenSource cts = new CancellationTokenSource();
            CancellationToken cancellationToken = cts.Token;

            Console.CancelKeyPress += (sender, e) =>
            {
                e.Cancel = true;
                cts.Cancel();
            };

            Task task = new Task(() =>
            {
                try
                {
                    var n = SwampSort.Sort(ints, cancellationToken);
                    foreach (int i in ints)
                    {
                        Console.Write($"{i} ");
                    }
                    Console.WriteLine($"\nИтераций ушло: {n}");
                }
                catch (OperationCanceledException)
                {
                    Console.WriteLine("\nОперация была отменена пользователем.");
                }
            }, cancellationToken);

            stopwatch.Start();
            task.Start();

            while (!task.IsCompleted)
            {
                Console.WriteLine($"Статус задачи: {task.Status}");
                System.Threading.Tasks.Task.Delay(100).Wait();
            }

            stopwatch.Stop();

            Console.WriteLine($"Статус задачи: {task.Status}");
            Console.WriteLine($"Время выполнения: {stopwatch.ElapsedMilliseconds} мс");
        }
    }
}
