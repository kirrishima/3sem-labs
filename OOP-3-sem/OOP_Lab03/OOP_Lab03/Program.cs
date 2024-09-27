namespace OOP_Lab03
{
    class Program
    {
        static void Main(string[] args)
        {
            int[] values1 = { 1, 2, 3, 4, 5 };
            int[] values2 = { 6, 7, 8, 9, 10 };

            Array array1 = new Array(values1);
            Array array2 = new Array(values2);

            Console.WriteLine($"array1: {array1}");
            Console.WriteLine($"array2: {array2}");

            Array sumArray = array1 + array2;
            Console.WriteLine($"\narray1 + array2 {sumArray}");

            Array diffArray = array1 - 2;
            Console.WriteLine($"\narray1 - 2: {diffArray}");

            bool contains = array1 > 3;
            Console.WriteLine($"\narray1 > 3: {contains}");

            bool doesntContains = array1 < 10;
            Console.WriteLine($"\narray1 < 10: {doesntContains}");

            bool areEqual = array1 == array2;
            Console.WriteLine($"\narray1 == array2: {areEqual}");

            bool areNotEqual = array1 != array2;
            Console.WriteLine($"\narray1 != array2: {areNotEqual}");


            Array testArray = new([10, 20, 30, 40, 50]);
            Console.WriteLine($"\n�������� ������: {testArray}");

            int sum = StatisticOperation.Sum(testArray);
            Console.WriteLine($"\n����� ��������� �������: {sum}");

            int maxMinDiff = StatisticOperation.MaxMinDiff(testArray);
            Console.WriteLine($"\n������� ����� ������������ � ����������� ����������: {maxMinDiff}");

            int count = StatisticOperation.Count(testArray);
            Console.WriteLine($"\n���������� ��������� � �������: {count}");

            string testString = "������� �����";
            string actualResult = StatisticOperation.DeleteVolwes(testString);

            Console.WriteLine($"\n�������� ������: {testString}");
            Console.WriteLine($"���������: {actualResult}");


            var p = new Array.Production(12, "������");

            var dev = new Array.Developer("������� ������ ��������", 1, "GameDev");
        }
    }
}
