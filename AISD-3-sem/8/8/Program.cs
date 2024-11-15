class Program
{
    struct Item
    {
        public string Name;
        public int Weight;
        public int Cost;
    }
    static int Knapsack(int maxWeight, List<Item> items)
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

        return -1;
    }

    static void PrintTable(int[,] table, int maxWeight, List<Item> items)
    {
        Console.WriteLine("В рюкзаке: ");
        for (int i = items.Count; i > 0; i--)
        {
            if (table[i - 1, maxWeight] != table[i, maxWeight])
            {
                Console.WriteLine($"Название: {items[i - 1].Name}. Масса: {items[i - 1].Weight}. Цена: {items[i - 1].Cost}");
                maxWeight -= items[i - 1].Weight;
            }
        }
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
        Console.WriteLine($"Максимальная вместительность: {N}");
        Console.WriteLine("\nСписок товаров:");
        foreach (var item in items)
        {
            Console.WriteLine($"Название: {item.Name}, Вес: {item.Weight}, Стоимость: {item.Cost}");
        }

        int maxWeight = 7;
        var c = Knapsack(maxWeight, items);
    }
}
