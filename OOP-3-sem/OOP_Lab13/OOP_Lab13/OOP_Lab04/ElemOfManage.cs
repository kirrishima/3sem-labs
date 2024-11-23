using System.Drawing;
using System.Text.Json.Serialization;

namespace OOP_Lab13.OOP_Lab04
{
    [Serializable]
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
