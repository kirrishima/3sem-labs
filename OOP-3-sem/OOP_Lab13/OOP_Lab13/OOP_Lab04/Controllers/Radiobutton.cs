using OOP_Lab13.Interfaces;

namespace OOP_Lab13.OOP_Lab04.Controllers
{
    public sealed class Radiobutton : ElemOfManage, IManagement
    {
        public void Show()
        {
            Console.WriteLine("Это Radiobutton");
        }

        public void Input()
        {
            if (Tap)
            {
                Console.WriteLine("Radiobutton отмечен ");
            }
            else
            {
                Console.WriteLine("Radiobutton не отмечен ");
            }
        }

        public override void Resize(double x, double y)
        {
            PointX = x;
            PointY = y;
        }
    }
}
