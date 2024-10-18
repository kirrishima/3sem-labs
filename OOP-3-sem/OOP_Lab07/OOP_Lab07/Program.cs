using OOP_Lab03;

namespace OOP_Lab03
{
    class Program
    {

        static void Main(string[] args)
        {
            char[] arr = ['a', 's'];
            OOP_Lab03.Array<char> array = new(new char[] { 'a', 'b', 'o', 'b', 'a' });

            OOP_Lab03.Array<int> array2 = new(new int[] { 1, 2, 3, 4, 5, 6, 7 });

            Console.WriteLine(array2);

            array2 = array2.Delete5First();

            Console.WriteLine(array2);
        }
    }
}
