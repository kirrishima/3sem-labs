using OOP_Lab13.Interfaces;
using OOP_Lab13.OOP_Lab04.Controllers;

namespace OOP_Lab13.OOP_Lab04.Figures
{
    public class Circle : Figure, IManagement
    {
        public double radius;
        public Checktbox? checktbox;
        public Radiobutton? radiobutton;

        Radiobutton newbutton = new Radiobutton();

        public Circle(double pointX, double pointY, double radius, ElemOfManage button)
        {

            this.pointX = pointX;
            this.pointY = pointY;
            this.radius = radius;

            checktbox = button as Checktbox;
            radiobutton = button as Radiobutton;

            if (button as Button is not null)
            {
                Console.WriteLine("Circle не может быть Button");
            }
        }

        public override string ToString()
        {
            return $"pointX: {pointX}, pointY: {pointY}, Radius: {radius}";
        }

        public override bool Equals(object? obj)
        {
            if (obj == null)
            {
                return false;
            }
            return (obj as Circle)?.radius == radius;
        }

        public override int GetHashCode()
        {
            return (int)radius;
        }

        void IManagement.Show()
        {
            Console.WriteLine("Координаты кнопки: {0}, {1}", pointX, pointY);
            Console.WriteLine("Радиус кнопки: {0}", radius);
        }

        public override void Show()
        {
            if (checktbox != null)
            {
                checktbox.Show();
                Console.WriteLine("Радиус кнопки: {0}", radius);
            }

            if (radiobutton != null)
            {
                radiobutton.Show();
                Console.WriteLine("Радиус кнопки: {0}", radius);
            }

            if (checktbox == null && radiobutton == null)
            {
                Console.WriteLine("Эта кнопка не является кругом ");
            }

            Console.WriteLine("Координаты кнопки: {0}, {1}", pointX, pointY);
            Console.WriteLine(newbutton);
        }

        public void Input()
        {
            Console.WriteLine("Это класс круг\n");
        }

        public void Resize(double x, double y)
        {
            pointX = x;
            pointY = y;
        }
    }
}
