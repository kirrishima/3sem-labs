using OOP_Lab05.Controllers;
using OOP_Lab05.Controllers.Interfaces;
using OOP_Lab05.Shapes;

namespace OOP_Lab04
{
    public sealed class Printer
    {
        public static void IAmPrinting(IManagement obj)
        {
            if (obj is Figure)
            {
                Console.WriteLine($"Type object({typeof(Figure)}): " + obj);
            }
            else if (obj as ManageElement is not null)
            {
                var tmp = obj as ManageElement;
                Console.WriteLine($"Type object({tmp?.GetType()}): " + tmp);
            }
        }
    }
}
