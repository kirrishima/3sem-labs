using OOP_Lab05.Controllers.Interfaces;

namespace OOP_Lab05.Controllers
{
    public sealed class Button : ManageElement, IManagement
    {
        public double Width { get; set; } = 0;
        public double Height { get; set; } = 0;

        public void Show()
        {
            Console.WriteLine($"Button [{PointX}, {PointY}]");
        }

        public void Input()
        {
            if (Tap)
            {
                Console.WriteLine("кнопка нажата");
            }
            else
            {
                Console.WriteLine("кнопка не нажата");
            }
        }

        public override void Resize(double x, double y)
        {
            PointX = x;
            PointY = y;
        }
    }
}

