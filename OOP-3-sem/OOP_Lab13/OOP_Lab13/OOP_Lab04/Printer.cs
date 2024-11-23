using OOP_Lab13.Interfaces;
using OOP_Lab13.OOP_Lab04.Figures;

namespace OOP_Lab13.OOP_Lab04
{
    public sealed class Printer
    {
        public static void IAmPrinting(IManagement obj)
        {
            if (obj is Figure)
            {
                Console.WriteLine($"Type object({typeof(Figure)}): " + obj);
            }
            else if (obj as ElemOfManage is not null)
            {
                var tmp = obj as ElemOfManage;
                Console.WriteLine($"Type object({tmp?.GetType()}): " + tmp);
            }
        }
    }
}
