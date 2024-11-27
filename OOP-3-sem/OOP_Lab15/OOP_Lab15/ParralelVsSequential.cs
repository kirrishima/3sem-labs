using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OOP_Lab15
{
    internal static class ParralelVsSequential
    {
        public static void Compare()
        {
            int arraySize = 1_000_000;
            int numArrays = 5;

            Console.WriteLine("Последовательное выполнение: ");
            MeasureExecutionTime(() => SequentialProcessing(arraySize, numArrays));

            Console.WriteLine("Параллельное выполнение: ");
            MeasureExecutionTime(() => ParallelProcessing(arraySize, numArrays));
        }

        static void SequentialProcessing(int arraySize, int numArrays)
        {
            for (int i = 0; i < numArrays; i++)
            {
                var array = GenerateArray(arraySize);
                var processedArray = ProcessArray(array);
                Array.Sort(processedArray);
            }
        }

        static void ParallelProcessing(int arraySize, int numArrays)
        {
            Parallel.For(0, numArrays, i =>
            {
                var array = GenerateArray(arraySize);
                var processedArray = ProcessArray(array);
                Array.Sort(processedArray);
            });
        }

        public static int[] GenerateArray(int size)
        {
            var random = new Random();
            return Enumerable.Range(0, size).Select(_ => random.Next(1, 1000)).ToArray();
        }

        static int[] ProcessArray(int[] array)
        {
            return array.Select(x => x * x).ToArray();
        }

        static void MeasureExecutionTime(Action action)
        {
            var stopwatch = Stopwatch.StartNew();
            action();
            stopwatch.Stop();
            Console.WriteLine($"Время выполнения: {stopwatch.ElapsedMilliseconds} мс\n");
        }
    }
}

