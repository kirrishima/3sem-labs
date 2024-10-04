namespace OOP_Lab04
{
    public sealed class Checktbox : ElemOfManage, IManagement
    {
        public void Show()
        {
            Console.WriteLine("Это ChecktBox ");
        }
        public void Input()
        {
            if (Tap)
            {
                Console.WriteLine("ChecktBox отмечен");
            }
            else
            {
                Console.WriteLine("ChecktBox ототмечен");
            }

        }

        public void Resize(double x, double y)
        {
            PointX = x;
            PointY = y;
        }
    }
}
