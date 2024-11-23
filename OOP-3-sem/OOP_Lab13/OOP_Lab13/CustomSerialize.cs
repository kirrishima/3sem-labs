using System.Text.Json;
using System.Xml.Serialization;

namespace OOP_Lab04
{
    public class CustomSerialize
    {
        public static void SerializeToXML(Rectangle rectangle)
        {
            XmlSerializer xSer = new XmlSerializer(typeof(Rectangle));

            using (FileStream fs = new FileStream("pointsx.xml", FileMode.OpenOrCreate))
            {
                xSer.Serialize(fs, rectangle);
            }
        }

        public static void DeserializeFromXML()
        {
            XmlSerializer xSer = new XmlSerializer(typeof(Rectangle));

            using (FileStream fs = new FileStream("pointsx.xml", FileMode.OpenOrCreate))
            {
                Rectangle newR = xSer.Deserialize(fs) as Rectangle;
                Console.WriteLine(newR.ToString());
            }
        }


        public static void SerializeToJSON(Rectangle rectangle)
        {
            string jsonString = JsonSerializer.Serialize(rectangle);
            File.WriteAllText("points.json", jsonString);
        }

        public static void DeserializeFromJSON()
        {
            string jsonString = File.ReadAllText("points.json");
            Rectangle newR = JsonSerializer.Deserialize<Rectangle>(jsonString);
            Console.WriteLine(newR.ToString());
        }

    }
}
