using OOP_Lab05.Controllers;
using OOP_Lab05.Controllers.Interfaces;
using OOP_Lab05.Shapes;
using OOP_Lab06.Exceptions;

namespace OOP_Lab04
{
    public sealed class Printer
    {
        public static void IAmPrinting(IManagement obj)
        {
            OOP6Exception.ThrowIfNull(obj, $"{typeof(IManagement)} obj");

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
