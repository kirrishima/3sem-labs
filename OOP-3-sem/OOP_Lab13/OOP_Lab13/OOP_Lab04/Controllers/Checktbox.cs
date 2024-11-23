using OOP_Lab13.Interfaces;

namespace OOP_Lab13.OOP_Lab04.Controllers
{
    [Serializable]
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

        public override void Resize(double x, double y)
        {
            PointX = x;
            PointY = y;
        }
    }
}
