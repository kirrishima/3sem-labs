namespace OOP_Lab04
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
            Checktbox newbutton1 = button as Checktbox;
            checktbox = newbutton1;
            if (checktbox == null)
            {
                Radiobutton newbutton2 = button as Radiobutton;
                radiobutton = newbutton2;
                if (radiobutton == null)
                {
                    Button radioButton1 = button as Button;
                    if (radioButton1 != null)
                    {
                        Console.WriteLine("Circle не может быть Button");
                    }
                }
            }
        }


        public override string ToString()
        {
            return $"pointX: {pointX}, pointY: {pointY}, Radius: {radius}";
        }

        public override bool Equals(object s)
        {
            if (s == null)
                return false;
            Circle temp = (Circle)s;
            return temp.radius == radius;
        }

        public override int GetHashCode()
        {
            return 123 * (int)radius;
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
