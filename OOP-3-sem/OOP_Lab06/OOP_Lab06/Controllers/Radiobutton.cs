using OOP_Lab05.Controllers.Interfaces;

namespace OOP_Lab05.Controllers
{
    public sealed class Radiobutton : ManageElement, IManagement
    {
        public double Radius { get; set; }

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
