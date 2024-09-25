//#define DISABLE_INPUT
using System;
using System.Reflection.Metadata;
using System.Text;

namespace OOP_Lab01
{
    internal class Program
    {
        private static void Main(string[] args)
        {

            // 1)типы
            bool bBool = false;
            byte byByte = 0;
            sbyte sbSByte = 0;
            char cChar = '\0';
            decimal decDecimal = 0.0m;
            double dDouble = 0.0;
            float fFloat = 0.0f;
            int iInt = 0;
            uint uiUInt = 0u;
            nint nInt = 0;
            nuint nuInt = 0u;
            long lLong = 0L;
            ulong ulULong = 0UL;
            short sShort = 0;
            ushort usUShort = 0;

            Console.WriteLine($"bool: {bBool}\n" +
                  $"byte: {byByte}\n" +
                  $"sbyte: {sbSByte}\n" +
                  $"char: {cChar}\n" +
                  $"decimal: {decDecimal}\n" +
                  $"double: {dDouble}\n" +
                  $"float: {fFloat}\n" +
                  $"int: {iInt}\n" +
                  $"uint: {uiUInt}\n" +
                  $"nint: {nInt}\n" +
                  $"nuint: {nuInt}\n" +
                  $"long: {lLong}\n" +
                  $"ulong: {ulULong}\n" +
                  $"short: {sShort}\n" +
                  $"ushort: {usUShort}\n");

#if !DISABLE_INPUT
            Console.WriteLine("bool:");
            while (!bool.TryParse(Console.ReadLine(), out bBool)) ;
            Console.WriteLine($"bool: {bBool}\n");

            Console.WriteLine("byte:");
            while (!byte.TryParse(Console.ReadLine(), out byByte)) ;
            Console.WriteLine($"byte: {byByte}\n");

            Console.WriteLine("sbyte:");
            while (!sbyte.TryParse(Console.ReadLine(), out sbSByte)) ;
            Console.WriteLine($"sbyte: {sbSByte}\n");

            Console.WriteLine("char:");
            while (!char.TryParse(Console.ReadLine(), out cChar)) ;
            Console.WriteLine($"char: {cChar}\n");

            Console.WriteLine("decimal:");
            while (!decimal.TryParse(Console.ReadLine(), out decDecimal)) ;
            Console.WriteLine($"decimal: {decDecimal}\n");

            Console.WriteLine("double:");
            while (!double.TryParse(Console.ReadLine(), out dDouble)) ;
            Console.WriteLine($"double: {dDouble}\n");

            Console.WriteLine("float:");
            while (!float.TryParse(Console.ReadLine(), out fFloat)) ;
            Console.WriteLine($"float: {fFloat}\n");

            Console.WriteLine("int:");
            while (!int.TryParse(Console.ReadLine(), out iInt)) ;
            Console.WriteLine($"int: {iInt}\n");

            Console.WriteLine("uint:");
            while (!uint.TryParse(Console.ReadLine(), out uiUInt)) ;
            Console.WriteLine($"uint: {uiUInt}\n");

            Console.WriteLine("nint:");
            while (!nint.TryParse(Console.ReadLine(), out nInt)) ;
            Console.WriteLine($"nint: {nInt}\n");

            Console.WriteLine("nuint:");
            while (!nuint.TryParse(Console.ReadLine(), out nuInt)) ;
            Console.WriteLine($"nuint: {nuInt}\n");

            Console.WriteLine("long:");
            while (!long.TryParse(Console.ReadLine(), out lLong)) ;
            Console.WriteLine($"long: {lLong}\n");

            Console.WriteLine("ulong:");
            while (!ulong.TryParse(Console.ReadLine(), out ulULong)) ;
            Console.WriteLine($"ulong: {ulULong}\n");

            Console.WriteLine("short:");
            while (!short.TryParse(Console.ReadLine(), out sShort)) ;
            Console.WriteLine($"short: {sShort}\n");

            Console.WriteLine("ushort:");
            while (!ushort.TryParse(Console.ReadLine(), out usUShort)) ;
            Console.WriteLine($"ushort: {usUShort}\n");
#endif
            // неявное приведение
            short impShort = sbSByte;
            int impInt = impShort;
            long impLong = impInt;
            float impFloat = impLong;
            double impDouble = impFloat;

            // явное приведение
            impShort = (short)byByte;
            impInt = (int)impShort;
            impLong = (long)impInt;
            impFloat = (float)impLong;
            impDouble = (double)impFloat;

            // упаковка и распаковка
            int x = 123;
            object objInt = x;
            int unpackedInt = (int)objInt;

            bool b = true;
            object objBool = b;
            bool unpackedBool = (bool)objBool;

            double d = 3.14;
            object objDouble = d;
            double unpackedDouble = (double)objDouble;

            // работа с неявно типизированной переменной
            var varInt = new int[] { 12, 23, 34 };
            foreach (var item in varInt)
            {
                Console.Write(item + " ");
            }
            Console.WriteLine($"\n{varInt.GetType()}\n");

            //работа с nullable переменной
            string? nullableString = null;
            if (nullableString == null)
            {
                Console.WriteLine("Var is null");
            }
            nullableString ??= "String was null but is not now :)\n";
            Console.WriteLine(nullableString);

            // присвоение значение другого типа переменной типа var
            var varString = new string[] { " 12", "fldjhsdfk" };
            // varString = new int[] { 12, 23, 654 }; // - ошибка

            //Строки

            // строковые литералы
            string str1 = "Hello, world!";
            string str2 = "Hello, world!";

            Console.WriteLine($"str1 == str2: {str1 == str2}");
            Console.WriteLine($"String.Equals(str1, str2): {String.Equals(str1, str2)}");
            Console.WriteLine($"String.Compare(str1, str2): {String.Compare(str1, str2)}");

            //строки на основе String
            String Str1 = new string(str1);
            String Str2 = new string(str2);

            //сцепление
            Console.WriteLine($"\nString.Concat(Str1, Str2):\n\t{String.Concat(Str1, Str2)}");
            //копирование
            Console.WriteLine($"\nString.Copy(Str1):\n\t{String.Copy(Str1)}");
            //выделение подстроки
            Console.WriteLine($"\nStr1.Substring(0, 5):\n\t{Str1.Substring(0, 5)}");
            //разделение строки на слова
            Console.WriteLine("\nSplit: ");
            foreach (var item in Str1.Split(", "))
            {
                Console.WriteLine($"\t{item}");
            }
            //вставки подстроки в заданную позицию
            Console.WriteLine($"\nStr2.Insert(0, \"Inserted: \"):\n\t{Str2.Insert(0, "Inserted: ")}");
            //удаление заданной подстроки
            Console.WriteLine($"\nStr2.Remove(0, 7):\n\t{Str2.Remove(0, 7)}\n");

            //string.IsNullOrEmpty
            string? nullStr = null;
            Console.WriteLine($"\nString.IsNullOrEmpty(nullStr): {String.IsNullOrEmpty(nullStr)}");
            nullStr = nullStr ?? "A";
            Console.WriteLine($"String.IsNullOrWhiteSpace(nullStr): {String.IsNullOrWhiteSpace(nullStr)}\n");

            //StringBuilder
            Console.WriteLine("StringBuilder\n");
            StringBuilder sb = new StringBuilder("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
            Console.WriteLine(sb);
            sb.Remove(sb.Length / 2, 5);
            Console.WriteLine(sb);
            sb.Insert(0, ['А', 'Б', 'В', 'Г', 'Д', ' ']);
            Console.WriteLine(sb);
            sb.Append(" new chars added");
            Console.WriteLine(sb);

            //Массивы
            Console.WriteLine("\nМассивы\n");
            //целый двумерный массив
            int[,] ints = { { 1, 2, 3, 4 }, { 5, 6, 7, 8 } };

            for (int i = 0; i < ints.GetLength(0); i++)
            {
                for (int j = 0; j < ints.GetLength(1); j++)
                {
                    Console.Write($"{ints[i, j]} ");
                }
                Console.WriteLine();
            }
            //одномерный массив строк
            string[] strings = { "abc", "def", "ghi", "jkl" };

            foreach (var item in strings)
            {
                Console.Write($"{item} ");
            }
            Console.WriteLine($"\nLength: {strings.Length}");
#if !DISABLE_INPUT
            Console.Write("Enter the index: ");
            int index = int.Parse(Console.ReadLine());
            Console.Write("Enter new string: ");
            string newValue = Console.ReadLine();

            strings[index] = newValue;

            foreach (var item in strings)
            {
                Console.Write($"{item} ");
            }
#endif
            //ступенчатый массив вещественных чисел
#if !DISABLE_INPUT
            Console.WriteLine("\n\nступенчатый массив вещественных чисел");
            double[][] doubles = [new double[2], new double[3], new double[4]];

            for (int i = 0; i < doubles.Length; i++)
            {
                for (int j = 0; j < doubles[i].Length; j++)
                {
                    Console.Write("Enter val: ");
                    while (!double.TryParse(Console.ReadLine(), out doubles[i][j]))
                    {
                        Console.Write("Incorrect input, retry ");
                    }
                }
            }
            for (int i = 0; i < doubles.Length; i++)
            {
                for (int j = 0; j < doubles[i].Length; j++)
                {
                    Console.Write($"{doubles[i][j]} ");
                }
                Console.WriteLine();
            }
#endif
            var varIntArr = new int[] { 1, 2, 3, 4 };
            var varStr = "String";

            //Кортежи
            Console.WriteLine("Кортежи");

            (int, string, char, string, ulong) tp = (21, "Biba", 'A', "Boba", 65645654645);
            Console.WriteLine(tp);
            Console.WriteLine("{0} и {1}", tp.Item2, tp.Item4);

            //распакоука
            var (tp1, tp2, tp3, tp4, tp5) = tp;
            Console.WriteLine($"tp1: {tp1}, tp2: {tp2}, tp3: {tp3}, tp4: {tp4}, tp5: {tp5}");
            var (_, tps1, _, tps2, _) = tp;
            Console.WriteLine($"tps1: {tps1}, tps2: {tps2}");

            //сравнение
            var tuple1 = (1, "Hello");
            var tuple2 = (1, "Hello");
            var tuple3 = (2, "World");

            Console.WriteLine(tuple1 == tuple2);
            Console.WriteLine(tuple1.Equals(tuple3));

            //Функции
            string input1 = "Force";
            int[] input2 = { 1, 2, 0, -1, 10 };

            var output1 = FindMinMax(input2, input1);
            Console.WriteLine($"{output1.max} {output1.min} {output1.sum} {output1.c}");

            (int max, int min, int sum, char c) FindMinMax(int[] ints, string str)
            {
                int min = int.MaxValue;
                int max = int.MinValue;
                int sum = 0;

                foreach (int item in ints)
                {
                    if (item < min)
                    {
                        min = item;
                    }
                    if (item > max)
                    {
                        max = item;
                    }
                    sum += item;
                }
                return (max, min, sum, str[0]);
            }

            void CheckedFunc()
            {
                checked
                {
                    int Max = int.MaxValue;
                    Console.WriteLine(Max);
                }
            }
            void Unchecked()
            {
                unchecked
                {
                    int Max = int.MaxValue;
                    Console.WriteLine(Max);
                }
            }
            CheckedFunc();
            Unchecked();

        }

    }
}