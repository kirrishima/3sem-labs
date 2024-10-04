using System.Drawing;

namespace OOP_Lab04
{
    public abstract class ElemOfManage
    {
        public bool Tap { get; set; }

        public double PointX { get; set; } = 0;
        public double PointY { get; set; } = 0;

        public abstract void Resize(double x, double y);

        public override string ToString()
        {
            return $"Это элемент управления";
        }
    }
}
