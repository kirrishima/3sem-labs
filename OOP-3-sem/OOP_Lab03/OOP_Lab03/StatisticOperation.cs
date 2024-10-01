using System.Runtime.CompilerServices;

namespace OOP_Lab03
{
    public static class StatisticOperation
    {
        static readonly char[] vowels = { 'а', 'е', 'ё', 'и', 'о', 'у', 'ы', 'э', 'ю', 'я' };

        public static int Sum(Array array)
        {
            return array.Data.Sum();
        }

        public static int MaxMinDiff(Array array)
        {
            return array.Data.Max() - array.Data.Min();
        }

        public static int Count(Array array)
        {
            return array.Length;
        }

        public static string DeleteVolwes(this string str)
        {
            return new string(str.Where(c => !vowels.Contains(char.ToLower(c))).ToArray());
        }

        public static Array Delete5First(this Array arr)
        {
            if (arr.Length <= 5)
            {
                return new Array(0);
            }

            int[] tmp = new int[arr.Length - 5];
            System.Array.Copy(arr.Data, arr.Length - 5, tmp, 0, arr.Length - 5);

            return new Array(tmp);
        }
    }
}
