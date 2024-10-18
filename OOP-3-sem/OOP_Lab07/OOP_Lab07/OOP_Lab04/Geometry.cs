using OOP_Lab04;

namespace OOP_Lab07
{
    internal class Geometry<C> where C : Figure, new()
    {
        C GetC = new C();

        public void What()
        {
            Console.WriteLine(GetC.GetType());
        }
    }
}
