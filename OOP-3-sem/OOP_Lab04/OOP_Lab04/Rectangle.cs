namespace OOP_Lab04
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
            this.pointX = pointX;
            this.pointY = pointY;
            this.width = width;
            this.height = height;

            Button newbutton1 = button as Button;
            this.button = newbutton1;
            if (this.button == null)
            {
                Checktbox newbutton2 = button as Checktbox;
                checktbox = newbutton2;
                if (this.button == null)
                {
                    Radiobutton radioButton = button as Radiobutton;
                    if (radioButton != null)
                    {
                        Console.WriteLine("Rectangle не может быть RadioButton");
                    }
                }
            }
        }

        public override string ToString()
        {
            return $"pointX: {pointX}, pointY: {pointY}, width: {width}, height: {height}";
        }

        void IManagement.Show()
        {

            Console.WriteLine("Координаты кнопки: {0}, {1}", pointX, pointY);

            Console.WriteLine("Ширина и высота кнопки: {0}, {1}", width, height);

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

            Console.WriteLine("Координаты кнопки: {0}, {1}", pointX, pointY);
            Console.WriteLine(newButton);
        }

        public void Input()
        {
            Console.WriteLine("\n\nЭто класс прямоугольник");
        }
    }
}
