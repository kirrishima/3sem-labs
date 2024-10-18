using System.Numerics;
using System.Runtime.CompilerServices;

namespace OOP_Lab03
{
    public static class StaticOperationString
    {
        static readonly char[] vowels = { 'а', 'е', 'ё', 'и', 'о', 'у', 'ы', 'э', 'ю', 'я' };

        public static string DeleteVolwes(this string str)
        {
            return new string(str.Where(c => !vowels.Contains(char.ToLower(c))).ToArray());
        }
    }

    public static class StaticOperationArray
    {
        public static int Sum(Array<int> array)
        {
            return array.Data.Sum();
        }

        public static int MaxMinDiff(Array<int> array)
        {
            return array.Data.Max() - array.Data.Min();
        }

        public static int Count(Array array)
        {
            return array.Length;
        }

        public static Array<T> Delete5First<T>(this Array<T> arr) where T : struct, ISubtractionOperators<T, T, T>
        {
            if (arr.Length <= 5)
            {
                return new Array<T>(0);
            }

            T[] tmp = new T[arr.Length - 5];
            System.Array.Copy(arr.Data, 5, tmp, 0, arr.Length - 5);

            return new Array<T>(tmp);
        }
    }
}
