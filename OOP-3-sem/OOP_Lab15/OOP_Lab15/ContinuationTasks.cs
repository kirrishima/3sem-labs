using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OOP_Lab15
{
    internal static class ContinuationTasks
    {
        public static void Tasks()
        {
            Task<int> task1 = Task.Run(() =>
            {
                Console.WriteLine("Task 1");
                Task.Delay(1000).Wait();
                return 1;
            });

            Task<int> task2 = Task.Run(() =>
            {
                Console.WriteLine("Task 2");
                Task.Delay(1500).Wait();
                return 2;
            });

            Task<int> task3 = Task.Run(() =>
            {
                Console.WriteLine("Task 3");
                Task.Delay(2000).Wait();
                return 3;
            });

            Task<int> aggregateTask = Task.WhenAll(task1, task2, task3).ContinueWith(t =>
            {
                int sum = t.Result.Sum();
                Console.WriteLine("Сумма результатов: " + sum);
                return sum;
            });

            Task continuation1 = aggregateTask.ContinueWith(t =>
            {
                Console.WriteLine("Первый продолженный таск " + (t.Result * 2));
            });

            Task continuation2 = Task.Run(() =>
            {
                var result = aggregateTask.GetAwaiter().GetResult();
                Console.WriteLine("Второй продолженный таск " + (result * 3));
            });

            Task.WaitAll(continuation1, continuation2);
        }
    }
}
