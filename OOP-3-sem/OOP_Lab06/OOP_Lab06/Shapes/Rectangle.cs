using OOP_Lab05.Controllers;
using OOP_Lab05.Controllers.Interfaces;
using OOP_Lab06.Exceptions;

namespace OOP_Lab05.Shapes
{
    public class Rectangle : Figure, IManagement
    {
        public double width;
        public double height;
        public Button? button;
        public Checktbox? checktbox;

        Button newButton = new Button();

        public Rectangle(double pointX, double pointY, double width, double height, IManagement button)
        {
            OOP6Exception.ThrowIfNull(pointX);
            OOP6Exception.ThrowIfNull(pointY);

            Point = new Utils.Point(pointX, pointY);
            this.width = width;
            this.height = height;


            this.button = button as Button;
            checktbox = button as Checktbox;

            if (this.button == null)
            {
                if (button as Radiobutton is not null)
                {
                    throw new OOP_Lab06.Exceptions.ArgumentException("Rectangle не может быть RadioButton");
                }
            }
            else
            {
                this.button.PointX = pointX;
                this.button.PointY = pointY;
            }
        }

        public override string ToString()
        {
            return $"pointX: {Point.X}, pointY: {Point.Y}, width: {width}, height: {height}";
        }

        void IManagement.Show()
        {
            Console.WriteLine("Координаты Rectangle: {0}, {1}", Point.X, Point.Y);
            Console.WriteLine("Ширина и высота Rectangle: {0}, {1}", width, height);
        }

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
