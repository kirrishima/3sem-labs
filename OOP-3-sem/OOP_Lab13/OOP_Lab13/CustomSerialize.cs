using System.Text.Json;
using System.Xml.Serialization;
using OOP_Lab13.OOP_Lab04.Figures;
using OOP_Lab13.Models;
using System.Runtime.Serialization.Formatters.Soap;

namespace OOP_Lab13
{
    public class CustomSerialize
    {
        public static void SerializeToXML(Rectangle rectangle)
        {
            XmlSerializer xSer = new(typeof(Rectangle));

            using (FileStream fs = new(Pathes.SerializedXML, FileMode.Create))
            {
                xSer.Serialize(fs, rectangle);
            }
        }

        public static void DeserializeFromXML()
        {
            XmlSerializer xSer = new(typeof(Rectangle));

            using (FileStream fs = new(Pathes.SerializedXML, FileMode.OpenOrCreate))
            {
                Rectangle? newR = xSer.Deserialize(fs) as Rectangle;
                Console.WriteLine(newR?.ToString());
            }
        }

        public static void SerializeToJSON(Rectangle rectangle)
        {
            var options = new JsonSerializerOptions
            {
                WriteIndented = true,
                IncludeFields = true
            };

            string jsonString = JsonSerializer.Serialize(rectangle, options);
            File.WriteAllText("points.json", jsonString);
        }

        public static void DeserializeFromJSON()
        {
            var options = new JsonSerializerOptions
            {
                WriteIndented = true,
                IncludeFields = true
            };

            string jsonString = File.ReadAllText("points.json");
            Rectangle? newR = JsonSerializer.Deserialize<Rectangle>(jsonString, options);
            Console.WriteLine(newR?.ToString());
        }

        public static void SerializeToSOAP(Rectangle rectangle)
        {
            SoapFormatter formatter = new SoapFormatter();

            using (FileStream fs = new FileStream("points.xml", FileMode.OpenOrCreate))
            {
                formatter.Serialize(fs, rectangle);
            }
        }

        public static void DeserializeFromSOAP()
        {
            SoapFormatter formatter = new SoapFormatter();

            using (FileStream fs = new FileStream("points.xml", FileMode.OpenOrCreate))
            {
                Rectangle newR = formatter.Deserialize(fs) as Rectangle;
                Console.WriteLine(newR.ToString());
            }
        }
    }
}
