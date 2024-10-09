using OOP_Lab05.Controllers;
using OOP_Lab05.Controllers.Interfaces;
using OOP_Lab05.Controllers.UI;
using OOP_Lab05.Shapes;

namespace OOP_Lab04
{
    class Program
    {
        public static void Main()
        {
            /*            UIController uI = new();
                        List<ManageElement> list = [new Button() { Width = 12, Height = 10 }, new Checktbox() { Side = 5 }, new Radiobutton() { Radius = 3 }];

                        uI.PrintAll();

                        uI.Elements = list;

                        uI.PrintAll();

                        Console.WriteLine($"{uI.CountArea():N}"); ;*/


            // Стандартные исключения

            try
            {
                int[] arr = [1, 2, 3, 3];
                Console.WriteLine(arr[10]);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"{ex.GetType()}: {ex.Message}");
            }
            Console.WriteLine();
            try
            {
                int x = 10;
                int y = 0;
                Console.WriteLine(x / y);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"{ex.GetType()}: {ex.Message}");
            }

            Console.WriteLine();
            try
            {
                dynamic d = new Button();
                List<int> list = new List<int>();
                list.Add(d);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"{ex.GetType()}: {ex.Message}");
            }
            Console.WriteLine();
            /*            Button NiceButton = new Button();

                        Checktbox NiceChecBox = new Checktbox();*/
            Radiobutton NiceRadioButton = new Radiobutton();

            /* Circle circleButtton = new Circle(4, 5, 4.4, NiceButton);
             Circle circleCheckBox = new Circle(11, 21, 3, NiceChecBox);
             Circle circleRadioButton = new Circle(2, 12, 6.3, NiceRadioButton);

             circleButtton.Input();
             circleButtton.Show();

             Console.WriteLine();

             circleCheckBox.checktbox.Tap = true;
             circleCheckBox.checktbox.Input();
             circleCheckBox.Show();

             Console.WriteLine();

             ((Figure)circleRadioButton).Show();
             circleRadioButton.radiobutton.Input();
             circleRadioButton.radiobutton.Tap = true;
             circleRadioButton.radiobutton.Input();

             Console.WriteLine("\nМетод из интерфейса: ");
             ((IManagement)circleRadioButton).Show();

             Console.WriteLine("\nРабота с прямоугольником");
             Console.WriteLine();

             Rectangle rectangle1 = new Rectangle(4.1, 4.1, 2, 7, NiceButton);
             Rectangle rectangle2 = new Rectangle(4.2, 4.2, 3, 8, NiceChecBox);*/
            Rectangle? rectangle3 = null;

            try
            {
                rectangle3 = new Rectangle(4.3, 4.3, 4, 9, NiceRadioButton);
            }
            catch (OOP_Lab06.Exceptions.ArgumentException ex)
            {
                Console.WriteLine($"Произошла ошибка: {ex.Message}");
            }

            /* Console.WriteLine();
             rectangle2.Input();

             Console.WriteLine();
             rectangle1.Show();

             Console.WriteLine();
             rectangle2.Show();*/

            Console.WriteLine();
            rectangle3?.Show();
            /*
                        Console.WriteLine();

                        Console.WriteLine("\nРабота с классом Print");

                        var printer = new IManagement[] { rectangle1, rectangle2, rectangle3, circleButtton, circleCheckBox, circleRadioButton };*/

            try
            {
                /*                foreach (var el in printer)
                                {
                                    Printer.IAmPrinting(el);
                                }*/
                Printer.IAmPrinting(null);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Произошла ошибка: {ex.Message}");
            }
        }
    }
}



