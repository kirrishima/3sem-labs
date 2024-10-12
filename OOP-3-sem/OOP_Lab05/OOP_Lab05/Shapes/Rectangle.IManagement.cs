namespace OOP_Lab05.Shapes
{
    public partial class Rectangle
    {
        public override void Show()
        {

            if (button != null)
            {
                button.Show();
                Console.WriteLine("Ширина кнопки: {0}", width);
                Console.WriteLine("Высота кнопки: {0}", height);
            }

            if (checktbox != null)
            {
                checktbox.Show();
                Console.WriteLine("Ширина кнопки: {0}", width);
                Console.WriteLine("Высота кнопки: {0}", height);
            }

            if (button == null && checktbox == null)
            {
                Console.WriteLine("\nНе является прямоугольной кнопкой ");
            }

            Console.WriteLine("Координаты кнопки: {0}, {1}", Point.X, Point.Y);
            Console.WriteLine(newButton);
        }

        public void Input()
        {
            Console.WriteLine("Это класс прямоугольник");
        }

        public void Resize(double x, double y)
        {
            Point = new Utils.Point(x, y);
        }
    }
}
