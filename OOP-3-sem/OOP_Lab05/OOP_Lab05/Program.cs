using OOP_Lab05.Controllers;
using OOP_Lab05.Controllers.Interfaces;
using OOP_Lab05.Controllers.UI;
using OOP_Lab05.Shapes;

namespace OOP_Lab04
{
    class Program
    {
        public static void Main()
        {
            UIController uI = new();
            List<ManageElement> list = [new Button() { Width = 12, Height = 10 }, new Checktbox() { Side = 5 }, new Radiobutton() { Radius = 3 }];

            uI.PrintAll();

            uI.Elements = list;

            uI.PrintAll();

            Console.WriteLine($"{uI.CountArea():N}"); ;
        }
    }
}



