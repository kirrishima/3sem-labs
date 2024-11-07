using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace OOP_Lab12
{
    class Program
    {
        static async Task Main(string[] args)
        {
            var data = new TestData
            {
                GameId = "ad7cd4cd-29d1-4548-89a3-91301996ef31",
                EarnedPoints = new EarnedPoints
                {
                    BP = new Point { Amount = 219 }
                },
                AssetClicks = new Dictionary<string, AssetClick>
                {
                    { "CLOVER", new AssetClick { Clicks = 74 } },
                    { "FREEZE", new AssetClick { Clicks = 3 } },
                    { "BOMB", new AssetClick { Clicks = 0 } },
                }
            };
            /* 
             * {
                "gameId": "157fdcd1-11d4-457d-a909-1232b82e6a93",
                "assets": {
                    "BOMB": {
                        "probability": "0.03",
                        "perClick": "1"
                    },
                    "CLOVER": {
                        "probability": "0.95",
                        "perClick": "1"
                    },
                    "FREEZE": {
                        "probability": "0.02",
                        "perClick": "1"
                    }
                }
            }*/
            string jsonString = JsonSerializer.Serialize(data, new JsonSerializerOptions { WriteIndented = true });
            var content = new StringContent(jsonString, Encoding.UTF8, "application/json");

            Console.WriteLine(content.ReadAsStringAsync().Result);
            Console.WriteLine("\n\n\n");

            using var client = new HttpClient();

            // Отправка POST-запроса
            HttpResponseMessage response = await client.PostAsync("https://blum-payload-generator.vercel.app/api/generate", content);

            // Чтение и вывод результата в виде строки
            string result = await response.Content.ReadAsStringAsync();
            Console.WriteLine(result);
        }
    }

    public class TestData
    {
        [JsonPropertyName("gameId")]
        public string GameId { get; set; }

        [JsonPropertyName("challenge")]
        public Challenge Challenge { get; set; }

        [JsonPropertyName("earnedPoints")]
        public EarnedPoints EarnedPoints { get; set; }

        [JsonPropertyName("assetClicks")]
        public Dictionary<string, AssetClick> AssetClicks { get; set; }
    }

    public class Challenge
    {
        [JsonPropertyName("id")]
        public string Id { get; set; }

        [JsonPropertyName("nonce")]
        public int Nonce { get; set; }

        [JsonPropertyName("hash")]
        public string Hash { get; set; }
    }

    public class EarnedPoints
    {
        [JsonPropertyName("BP")]
        public Point BP { get; set; }
    }

    public class Point
    {
        [JsonPropertyName("amount")]
        public int Amount { get; set; }
    }

    public class AssetClick
    {
        [JsonPropertyName("clicks")]
        public int Clicks { get; set; }
    }
}
