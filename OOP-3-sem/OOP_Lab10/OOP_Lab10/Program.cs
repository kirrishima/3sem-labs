using System;

namespace OOP_Lab10
{
    class Program
    {
        static void Main(string[] args)
        {
            string[] monthes = { "January", "February", "March", "April", "May", "June", "Jully", "August", "September", "October", "November", "December" };

            var a = from m in monthes where m.Length == 7 select m;

            foreach (var item in a)
            {
                Console.WriteLine(item);
            }
        }
    }
}