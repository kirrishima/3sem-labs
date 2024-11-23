using System.Xml;

namespace OOP_Lab04
{
    public static class Task3
    {
        public static void XPath()
        {
            XmlDocument xDoc = new XmlDocument();
            xDoc.Load(Path.Combine(Directory.GetCurrentDirectory(), "pointsx.xml"));
            XmlElement xRoot = xDoc.DocumentElement;

            Console.WriteLine("\n\n\n\t\t\t  XPath for XML:\n");
            Console.WriteLine("Все дочерние элементы:");
            XmlNodeList childNodes = xRoot.SelectNodes("*");

            foreach (XmlNode n in childNodes)
                Console.WriteLine(n.OuterXml);

            Console.WriteLine("\n<Width>:");
            XmlNodeList namesNodes = xRoot.SelectNodes("//Rectangle/width");

            foreach (XmlNode n in namesNodes)
                Console.WriteLine(n.InnerText);
        }
    }
}
