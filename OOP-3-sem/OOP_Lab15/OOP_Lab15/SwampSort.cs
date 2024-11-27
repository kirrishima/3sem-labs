namespace OOP_Lab15
{
    public class SwampSort
    {
        public static void Shuffle<T>(T[] array)
        {
            Random rng = new Random();
            int n = array.Length;
            while (n > 1)
            {
                int k = rng.Next(n--);
                T temp = array[n];
                array[n] = array[k];
                array[k] = temp;
            }
        }

        public static int Sort<T>(T[] array, CancellationToken? cancellationToken = null)
        {
            int count = 0;
            while (true)
            {
                if (cancellationToken?.IsCancellationRequested == true)
                {
                    Console.WriteLine("\nОтмена сортировки запрошена.");
                    return count;
                }

                if (array.SequenceEqual(array.OrderBy(x => x)) || array.SequenceEqual(array.OrderByDescending(x => x)))
                {
                    return count;
                }
                Shuffle(array);
                count++;
            }
        }
    }
}
