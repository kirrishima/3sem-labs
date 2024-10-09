using OOP_Lab05.Controllers.Interfaces;

namespace OOP_Lab05.Controllers
{
    public sealed class Checktbox : ManageElement, IManagement
    {
        public int Side { get; set; } = 0;

        public void Show()
        {
            Console.WriteLine("Это ChecktBox ");
        }

        public void Input()
        {
            Console.WriteLine("ChecktBox " + (Tap ? "отмечен" : "ототмечен"));
        }

        public override void Resize(double x, double y)
        {
            PointX = x;
            PointY = y;
        }
    }
}
