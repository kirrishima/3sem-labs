using System.Xml.Linq;
using OOP_Lab13.OOP_Lab04.Controllers;
using OOP_Lab13.OOP_Lab04.Figures;

namespace OOP_Lab13
{
    class Program
    {
        public static void Main()
        {
            var button = new Button() { PointX = 52, PointY = 66, Tap = true };
            Rectangle rectangle = new Rectangle(4.1, 4.1, 2, 7, button);
            CustomSerialize.SerializeToXML(rectangle);
            CustomSerialize.DeserializeFromXML();

            CustomSerialize.SerializeToJSON(rectangle);
            CustomSerialize.DeserializeFromJSON();

            Console.WriteLine("\nЗадание 2: \n");

            Rectangle rectangle2 = new Rectangle(1, 4, 4, 5, null);

            Rectangle[] rectangles = { rectangle, rectangle2 };


            MyXPath.XPath();

            XDocument xdoc = LINQ_ToXML.CreateXML();
            LINQ_ToXML.CoutXML(xdoc);
            LINQ_ToXML.LinqXML(xdoc);
        }
    }
}



