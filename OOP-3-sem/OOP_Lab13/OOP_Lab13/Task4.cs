using System.Xml.Linq;

namespace OOP_Lab04
{
    class Task4
    {
        public static XDocument CreateXML()
        {
            XDocument xdoc = new XDocument(new XElement("phones",
                new XElement("phone",
                    new XAttribute("name", "iPhone 52 pro"),
                    new XElement("company", "Apple"),
                    new XElement("price", "455000 денег")
                    ),
                new XElement("phone",
                    new XAttribute("name", "Pixel 9 pro"),
                    new XElement("company", "Google"),
                    new XElement("price", "3000 денег")
                    )
                ));
            xdoc.Save(Path.Combine(Directory.GetCurrentDirectory(), "xxml.xml"));
            return xdoc;
        }

        public static void CoutXML(XDocument xdoc)
        {
            Console.WriteLine("\n\n\t\t\t   Linq to XML:");
            foreach (XElement phoneElement in xdoc.Element("phones").Elements("phone"))
            {
                XAttribute nameAttribute = phoneElement.Attribute("name");
                XElement companyElement = phoneElement.Element("company");
                XElement priceElement = phoneElement.Element("price");

                if (nameAttribute != null && companyElement != null && priceElement != null)
                {
                    Console.WriteLine($"Смартфон: {nameAttribute.Value}");
                    Console.WriteLine($"Компания: {companyElement.Value}");
                    Console.WriteLine($"Цена: {priceElement.Value}");
                }
                Console.WriteLine();
            }
        }

        public static void LinqXML(XDocument xdoc)
        {
            var items = from xe in xdoc.Element("phones").Elements("phone")
                        where xe.Element("company").Value == "Google"
                        select new Phone
                        {
                            Name = xe.Attribute("name").Value,
                            Price = xe.Element("price").Value
                        };

            foreach (var item in items)
            {
                Console.WriteLine($"{item.Name} - {item.Price}");
            }

            var items1 = from xe in xdoc.Element("phones").Elements("phone")
                         where xe.Element("price").Value.Contains('4')
                         select new Phone
                         {
                             Name = xe.Attribute("name").Value,
                             Price = xe.Element("price").Value
                         };

            foreach (var item in items1)
            {
                Console.WriteLine($"{item.Name} - {item.Price}");
            }
        }
    }

    public class Phone
    {
        public string Name { get; set; }
        public string Price { get; set; }
    }
}

