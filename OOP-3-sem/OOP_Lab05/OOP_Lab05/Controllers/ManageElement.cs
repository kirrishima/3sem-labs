using OOP_Lab05.Utils;

namespace OOP_Lab05.Controllers
{
    public abstract class ManageElement
    {
        public bool Tap { get; set; }

        private Point point;

        public double PointX { get => point.X; set => point.X = value; }

        public double PointY { get => point.Y; set => point.Y = value; }

        public abstract void Resize(double x, double y);

        public override string ToString()
        {
            return $"Элемент управления ({GetType().Name})";
        }
    }
}
