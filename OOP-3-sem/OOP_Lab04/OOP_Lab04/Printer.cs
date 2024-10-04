namespace OOP_Lab04
{
    public sealed class Printer
    {
        public static void IAmPrinting(IManagement element)
        {
            if (element is Figure)
            {
                Console.WriteLine($"Type object({typeof(Figure)}): " + element);
            }
            else if (element as ElemOfManage is not null)
            {
                var temp = element as ElemOfManage;
                Console.WriteLine($"Type object({temp.GetType()}): " + temp);
            }
        }
    }
}
