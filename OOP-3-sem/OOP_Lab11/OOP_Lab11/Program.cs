using System;
using System.Reflection;

namespace OOP_Lab11
{
    public class MyClass
    {
        public void TestMethod(int x, string y)
        {
            Console.WriteLine($"x: {x}, y: {y}");
        }
    }

    class Program
    {
        private static Assembly Assembly { get; set; } = Assembly.LoadFrom("OOP_Lab03.dll"); // dll из 3 лабы закинуть в bin\Debug\net8.0

        private static Type OOP3Array = Assembly.GetType("OOP_Lab03.Array");
        private static Type OOP3Static = Assembly.GetType("OOP_Lab03.StatisticOperation");


        public static void Print<T>(IEnumerable<T> collection)
        {
            Console.WriteLine(string.Join(", ", collection));
        }

        static void Main(string[] args)
        {

            Console.WriteLine(Reflector.GetAssemblyName(OOP3Array));
            Console.WriteLine(Reflector.GetAssemblyName(typeof(List<>)));
            Console.WriteLine();


            Console.WriteLine(Reflector.HasPublicConstructors(OOP3Static));
            Console.WriteLine(Reflector.HasPublicConstructors(typeof(List<>)));
            Console.WriteLine();


            Print(Reflector.GetMethods(OOP3Array));
            Print(Reflector.GetMethods(OOP3Static));
            Console.WriteLine();


            Print(Reflector.GetInterfaces(OOP3Array));
            Print(Reflector.GetInterfaces(typeof(List<>)));
            Console.WriteLine();


            Print(Reflector.GetFieldsAndProps(OOP3Array));
            Print(Reflector.GetFieldsAndProps(typeof(List<>)));
            Console.WriteLine();


            Reflector.PrintAllMethodsContainingParamsWithType(OOP3Array, typeof(int));
            Console.WriteLine();
            Reflector.PrintAllMethodsContainingParamsWithType(typeof(List<>), typeof(int));
            Console.WriteLine();

            // 1.g

            var myObject = Reflector.Create(OOP3Array, new int[] { 1, 2, 3, 4, 5, 6, 7, 78, 52, 8, 99, 0 });
            var methodName = "get_Item";

            Console.WriteLine();
            Console.WriteLine(Reflector.Invoke(myObject, methodName, [7]));

            Console.WriteLine();
            Console.WriteLine();

            myObject = Reflector.Create(typeof(MyClass));
            methodName = "TestMethod";

            var filePath = "params.txt";

            var method = typeof(MyClass).GetMethod(methodName);
            var parameters = Reflector.GetParametersFromFileOrGenerate(filePath, method);

            Reflector.Invoke(myObject, methodName, parameters);
        }
    }
}