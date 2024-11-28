class Program
{
    struct Item
    {
        public string Name;
        public int Weight;
        public int Cost;
    }

    static void Knapsack(int maxWeight, List<Item> items)
    {
        int n = items.Count;

        int[,] table = new int[n + 1, maxWeight + 1];

        for (int i = 1; i <= n; i++)
        {
            int currentWeight = items[i - 1].Weight;
            int currentCost = items[i - 1].Cost;

            for (int j = 0; j <= maxWeight; j++)
            {
                table[i, j] = table[i - 1, j];

                if (j >= currentWeight && table[i - 1, j - currentWeight] + currentCost > table[i, j])
                {
                    table[i, j] = table[i - 1, j - currentWeight] + currentCost;
                }
            }
        }
        PrintTable(table, maxWeight, items);
    }

    static void PrintTable(int[,] table, int maxWeight, List<Item> items)
    {
        Console.WriteLine("В рюкзаке: ");
        int cost = 0;
        for (int i = items.Count; i > 0; i--)
        {
            if (table[i - 1, maxWeight] != table[i, maxWeight])
            {
                Console.WriteLine($"Название: {items[i - 1].Name}. Масса: {items[i - 1].Weight}. Цена: {items[i - 1].Cost}");
                maxWeight -= items[i - 1].Weight;
                cost += items[i - 1].Cost;
            }
        }
        Console.WriteLine($"\nЦена рюкзака: {cost}");

        Console.WriteLine();
        Console.ReadLine();

        Console.Write("    ");
        for (int w = 0; w <= table.GetLength(1) - 1; w++)
        {
            Console.Write($"{w,4}");
        }
        Console.WriteLine();
        Console.WriteLine(new string('-', (table.GetLength(1)) * 4 + 4));
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
    }

    static void Main()
    {
        Console.Write("Введите количество товаров: ");
        int n = int.Parse(Console.ReadLine());
        Console.Write("Введите вместительность рюкзака: ");
        int N = int.Parse(Console.ReadLine());
        var items = new List<Item>();

        for (int i = 0; i < n; i++)
        {
            Console.WriteLine();
            Console.WriteLine($"Введите данные для товара {i + 1}:");
            Console.Write("Название: ");
            string name = Console.ReadLine();
            Console.Write("Вес: ");
            int weight = int.Parse(Console.ReadLine());
            Console.Write("Стоимость: ");
            int cost = int.Parse(Console.ReadLine());
            items.Add(new Item { Name = name, Weight = weight, Cost = cost });
        }

        Console.Clear();
        Console.WriteLine("Товары: ");
        foreach (var item in items)
        {
            Console.WriteLine($"Название: {item.Name}, Масса: {item.Weight}, Цена: {item.Cost}");
        }

        Console.WriteLine();
        Console.WriteLine($"Максимальная вместительность: {N}");
        Console.WriteLine();
        Knapsack(N, items);
    }
}
