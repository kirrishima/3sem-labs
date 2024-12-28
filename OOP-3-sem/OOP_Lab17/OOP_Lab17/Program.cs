public class Counter<T>
{
    public static int Count { get; private set; }

    public Counter()
    {
        Count++;
    }
}

class Program
{
    static void Main()
    {
        Counter<int> counter1 = new Counter<int>();
        Counter<int> counter2 = new Counter<int>();
        Counter<string> counter3 = new Counter<string>();

        Console.WriteLine(Counter<int>.Count);    // Вывод: 2
        Console.WriteLine(Counter<string>.Count); // Вывод: 1
    }
}
