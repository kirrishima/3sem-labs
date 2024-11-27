using OOP_Lab15;
using System.Diagnostics;

class Program
{
    static void Main(string[] args)
    {
        // 1-2
        //CancableTask.Task();

        //3
        //ContinuationTasks.Tasks();

        List<int> ints1 = new List<int>();
        List<int> ints2 = new List<int>();


        int c = 0;
        Stopwatch stopwatch = new Stopwatch();
        stopwatch.Start();

        for (int i = 0; i < 100000; i++) ints1.Add(i);

        stopwatch.Stop();

        Console.WriteLine($"{stopwatch.ElapsedMilliseconds}ms");
        Console.WriteLine();

        stopwatch = new Stopwatch();

        stopwatch.Start();
        Parallel.For(0, 100000, ints2.Add);
        stopwatch.Stop();
        Console.WriteLine($"{stopwatch.ElapsedMilliseconds}ms");
    }
}
