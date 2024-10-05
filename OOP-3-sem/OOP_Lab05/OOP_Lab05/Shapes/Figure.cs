using OOP_Lab05.Utils;

namespace OOP_Lab05.Shapes
{
    public abstract class Figure
    {
        public readonly Color color;

        public Point Point { get; set; }

        public override string ToString()
        {
            return $"X: {Point.X}, Y: {Point.Y}. Color: {color}";
        }

        public abstract void Show();
    }
}
