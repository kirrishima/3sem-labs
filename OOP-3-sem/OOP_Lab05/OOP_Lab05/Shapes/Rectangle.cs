using OOP_Lab05.Controllers;
using OOP_Lab05.Controllers.Interfaces;

namespace OOP_Lab05.Shapes
{
    public partial class Rectangle : Figure, IManagement
    {
        public double width;
        public double height;
        public Button? button;
        public Checktbox? checktbox;

        Button newButton = new Button();

        public Rectangle(double pointX, double pointY, double width, double height, IManagement button)
        {
            Point = new Utils.Point(pointX, pointY);
            this.width = width;
            this.height = height;


            this.button = button as Button;
            checktbox = button as Checktbox;

            if (this.button == null)
            {
                if (button as Radiobutton is not null)
                {
                    Console.WriteLine("Rectangle не может быть RadioButton");
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
    }
}
