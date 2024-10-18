namespace OOP_Lab07
{
    class Program
    {
        static readonly string filepath = "test.txt";
        static void Main(string[] args)
        {
            OOP_Lab03.Array<char> array = new(['a', 'b', 'o', 'b', 'a']);

            Console.WriteLine(array);
            Console.WriteLine(array.GetType());

            OOP_Lab03.Array<int> array2 = new([1, 2, 3, 4, 5, 6, 7]);

            Console.WriteLine(array2);

            array2.SaveToFile(filepath);

            array2 = OOP_Lab03.StaticOperationArray.Delete5First(array2);

            Console.WriteLine(array2);

            Console.WriteLine(array2[0]);

            array2 = OOP_Lab03.Array<int>.LoadFromFile(filepath);
            Console.WriteLine(array2);

            try
            {
                Console.WriteLine(array2[10]);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
            finally
            {
                Console.WriteLine(array2.Length);
                array2.Show();
            }

            array2.Add(1);
            array2.Show();

            array2.Delete(1);
            array2.Show();

            array2.Delete(0);
            array2.Show();

            Console.WriteLine('\n');


            Geometry<OOP_Lab04.Circle> geometry = new();
            geometry.What();

            // Geometry<OOP_Lab04.Rectangle> geometry2 = new();
        }
    }
}
