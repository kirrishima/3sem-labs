using System.Xml;
using OOP_Lab13.Models;

namespace OOP_Lab13
{
    public static class MyXPath
    {
        public static void XPath()
        {
            XmlDocument xDoc = new XmlDocument();
            xDoc.Load(Pathes.SerializedXML);
            XmlElement xRoot = xDoc.DocumentElement;

            Console.WriteLine("XPath for XML:\n");
            Console.WriteLine("Все дочерние элементы:");
            XmlNodeList childNodes = xRoot.SelectNodes("*");

            foreach (XmlNode n in childNodes)
            {
                Console.WriteLine(n.OuterXml);
            }

            Console.WriteLine("\n<button>:");
            XmlNodeList buttonNodes = xRoot.SelectNodes("//Rectangle/button");

            foreach (XmlNode n in buttonNodes)
            {
                Console.WriteLine(n.InnerText);
            }
        }
    }
}
