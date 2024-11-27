using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OOP_Lab15
{
    internal static class ParallelInvoke
    {
        public static void Test()
        {
            int[] array = ParralelVsSequential.GenerateArray(10);

            Parallel.Invoke(
                () =>
                {
                    Console.WriteLine("\nПервый блок");
                    var querry = array.Where(x => x % 2 == 0);
                    Task.Delay(1000).Wait();
                    foreach (var item in querry)
                    {
                        Console.WriteLine(item);
                    }
                    Console.WriteLine();
                },
                () =>
                {
                    Console.WriteLine("\nВторой блок");
                    var querry = array.OrderBy(x => x);
                    Task.Delay(1200).Wait();
                    foreach (var item in querry)
                    {
                        Console.WriteLine(item);
                    }
                    Console.WriteLine();
                },
                () =>
                {
                    Console.WriteLine("\nТретий блок");
                    var querry = array.Take(5);
                    Task.Delay(1400).Wait();
                    foreach (var item in querry)
                    {
                        Console.WriteLine(item);
                    }
                    Console.WriteLine();
                }
            );
        }
    }
}
