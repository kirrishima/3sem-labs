using System.Xml.Linq;

namespace OOP_Lab04
{

    class Program
    {

        public static void Main()
        {
            Rectangle rectangle = new Rectangle((float)4.1, (float)4.1, (float)2, (float)7);
            CustomSerialize.SerializeToXML(rectangle);
            CustomSerialize.DeserializeFromXML();
            CustomSerialize.SerializeToJSON(rectangle);
            CustomSerialize.DeserializeFromJSON();

            Console.WriteLine("\nЗадание 2: \n");
            Rectangle rectangle2 = new Rectangle((float)1, (float)4, (float)4, (float)5);

            Rectangle[] rectangles = { rectangle, rectangle2 };


            Task3.XPath();

            XDocument xdoc = Task4.CreateXML();
            Task4.CoutXML(xdoc);
            Task4.LinqXML(xdoc);
        }
    }
}



