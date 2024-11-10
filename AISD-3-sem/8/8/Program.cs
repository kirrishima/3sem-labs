struct Item
{
    public string Name;
    public int Weight;
    public int Cost;
}

class Program
{
    static int Knapsack(int maxWeight, List<Item> items)
    {
        int n = items.Count;

        int[,] table = new int[n + 1, maxWeight + 1];

        for (int i = 1; i <= n; i++)
        {
            int cw = items[i - 1].Weight;
            int cc = items[i - 1].Cost;

            for (int j = 0; j <= maxWeight; j++)
            {
                table[i, j] = table[i - 1, j];

                if (j >= cw && table[i - 1, j - cw] + cc > table[i, j])
                {
                    table[i, j] = table[i - 1, j - cw] + cc;
                }
            }
        }

        PrintTable(table, maxWeight, items);

        return -1;
    }

    static void PrintTable(int[,] table, int maxWeight, List<Item> items)
    {
        Console.Write("    ");
        for (int w = 0; w <= maxWeight; w++)
        {
            Console.Write($"{w,4}");
        }
        Console.WriteLine();

        Console.WriteLine(new string('-', (maxWeight + 1) * 4 + 4));

        for (int i = 0; i < table.GetLength(0); i++)
        {
            Console.Write($"{i,2} |");
            for (int j = 0; j < table.GetLength(1); j++)
            {
                Console.Write($"{table[i, j],4}");
            }
            Console.WriteLine();
        }

        Console.WriteLine();
        for (int i = items.Count; i > 0; i--)
        {
            if (table[i - 1, maxWeight] != table[i, maxWeight])
            {
                Console.WriteLine($"{items[i - 1].Name}. Масса: {items[i - 1].Weight}. Цена: {items[i - 1].Cost}");
                maxWeight -= items[i - 1].Weight;
            }
        }
    }

    static void Main()
    {
        var items = new List<Item>
        {
            new Item { Name = "1", Weight = 3, Cost = 2 },
            new Item { Name = "2", Weight = 1, Cost = 2 },
            new Item { Name = "3", Weight = 3, Cost = 4 },
            new Item { Name = "4", Weight = 4, Cost = 5 },
            new Item { Name = "5", Weight = 2, Cost = 3 }
        };

        int maxWeight = 7;
        var c = Knapsack(maxWeight, items);
    }
}
