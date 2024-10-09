namespace OOP_Lab05.Utils
{
    public struct Point
    {
        public double X { get; set; }
        public double Y { get; set; }

        public Point(double x, double y)
        {
            X = x;
            Y = y;
        }

        public Point(Point point)
        {
            X = point.X;
            Y = point.Y;
        }
    }

    public enum Color
    {
        Red = ConsoleColor.Red,
        Green = ConsoleColor.Green,
        Blue = ConsoleColor.Blue
    }
}