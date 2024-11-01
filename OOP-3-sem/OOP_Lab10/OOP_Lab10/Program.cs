using System;
using System.Net.Http;
using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Threading.Tasks;

namespace OOP_Lab10
{
    class Program
    {
        static async Task Main(string[] args)
        {
            using (HttpClient client = new())
            {
                BlumGameJson data = new()
                {
                    GameId = "ad7cd4cd-29d1-4548-89a3-91301996ef31",
                    EarnedAssets = new EarnedAssets
                    {
                        CLOVER = new Asset
                        {
                            Amount = "180"
                        }
                    }
                };

                // сериализуем объект BlumGameJson в строку JSON
                var jsonString = JsonSerializer.Serialize(data);

                // создаем объект StringContent для передачи в запросе
                var content = new StringContent(jsonString, Encoding.UTF8, "application/json");

                // отправляем POST-запрос
                var response = await client.PostAsync("https://test-wzs1.vercel.app/app/getPayload", content);

                // выводим ответ на консоль
                var responseContent = await response.Content.ReadAsStringAsync();
                Console.WriteLine(responseContent);
            }
        }
    }

    public class BlumGameJson
    {
        [JsonPropertyName("gameId")]
        public string? GameId { get; set; } = null;

        [JsonPropertyName("earnedAssets")]
        public EarnedAssets? EarnedAssets { get; set; } = null;
    }

    public class EarnedAssets
    {
        [JsonPropertyName("CLOVER")]
        public Asset? CLOVER { get; set; } = null;
    }

    public class Asset
    {
        [JsonPropertyName("amount")]
        public string? Amount { get; set; } = null;
    }
}
