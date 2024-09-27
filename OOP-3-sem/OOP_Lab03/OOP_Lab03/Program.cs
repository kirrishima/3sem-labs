using System;

namespace OOP_Lab03
{
    class Program
    {
        static void Main(string[] args)
        {
            Array array = new Array(2);
            array[0] = 1;
            array[1] = 2;

            Array array1 = new Array(3);

            array1[0] = 4;
            array1[1] = 5;
            array1[2] = 6;

            Console.WriteLine(array);
            Console.WriteLine(array1);
            Console.WriteLine(array + array1);
        }
    }
}