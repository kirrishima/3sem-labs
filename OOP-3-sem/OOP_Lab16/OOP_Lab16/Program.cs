using OfficeOpenXml;
using OfficeOpenXml.Style;
using System.Collections.Concurrent;
using System.Drawing;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Text.RegularExpressions;

class Program
{
    private static readonly HttpClient HttpClient = new();
    private static SemaphoreSlim ApiSemaphore;
    private static readonly object LockObject = new();
    private static ConcurrentDictionary<string, bool> VisitedUsers = new();
    private static List<string> Tokens;
    private static int CurrentTokenIndex = 0;
    private static readonly string VisistedFilPath = "VisitedUsers.json";

    private static int WritedRows = 0;

    static async Task Main(string[] args)
    {
        ExcelPackage.LicenseContext = OfficeOpenXml.LicenseContext.NonCommercial;
        var startUser = "okivlinas";
        var outputExcel = "github_analysis_results.xlsx";
        Tokens = new List<string>
        {
            "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",
            "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",
            "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",
            "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",
            "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",
            "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"
        };

        ApiSemaphore = new SemaphoreSlim(10, 10); // Max 10 concurrent requests

        if (File.Exists(VisistedFilPath))
        {
            VisitedUsers = await DeserializeConcurrentDictionaryAsync<string, bool>(VisistedFilPath);
        }

        await AnalyzeUserAsync(startUser, outputExcel, recursionLimit: 1, maxThreads: 12);

        await SerializeConcurrentDictionaryAsync(VisitedUsers, VisistedFilPath);
    }

    private static async Task SerializeConcurrentDictionaryAsync<TK, TV>(ConcurrentDictionary<TK, TV> dictionary, string filePath)
    {
        var serializedData = JsonSerializer.Serialize(dictionary);
        await File.WriteAllTextAsync(filePath, serializedData);
    }

    private static async Task<ConcurrentDictionary<TK, TV>> DeserializeConcurrentDictionaryAsync<TK, TV>(string filePath)
    {
        if (!File.Exists(filePath))
        {
            return new ConcurrentDictionary<TK, TV>();
        }

        var serializedData = await File.ReadAllTextAsync(filePath);
        var dictionary = JsonSerializer.Deserialize<ConcurrentDictionary<TK, TV>>(serializedData);

        return dictionary ?? new ConcurrentDictionary<TK, TV>();
    }


    private static async Task AnalyzeUserAsync(string username, string outputFile, int recursionLimit, int maxThreads)
    {
        using var package = new ExcelPackage();
        var sheet = package.Workbook.Worksheets.Add("GitHub Analysis");
        sheet.Cells[1, 1].Value = "User";
        sheet.Cells[1, 2].Value = "Repository Name";
        sheet.Cells[1, 3].Value = "Repository URL";

        // Применяем стиль к первой строке
        using (var range = sheet.Cells[1, 1, 1, 3]) // Диапазон первой строки (с 1-го столбца по 3-й)
        {
            range.Style.Font.Bold = true; // Жирный шрифт
            range.Style.Font.Size = 12;  // Размер шрифта
            range.Style.Font.Color.SetColor(Color.White); // Белый цвет текста
            range.Style.Fill.PatternType = ExcelFillStyle.Solid; // Заливка ячеек
            range.Style.Fill.BackgroundColor.SetColor(Color.DarkBlue); // Синий фон
            range.Style.HorizontalAlignment = ExcelHorizontalAlignment.Center; // Центрирование текста по горизонтали
            range.Style.VerticalAlignment = ExcelVerticalAlignment.Center; // Центрирование текста по вертикали
        }

        Console.CancelKeyPress += (sender, eventArgs) =>
        {
            Console.WriteLine("Termination signal received.");
            Console.WriteLine("Saving table...");
            SaveWorkbook(package, outputFile);
            SerializeConcurrentDictionaryAsync(VisitedUsers, VisistedFilPath).Wait();
            Environment.Exit(0);
        };

        Console.WriteLine($"Started parsing from start point {username}. Wait...");

        await AnalyzeRecursiveAsync(username, sheet, recursionLimit, maxThreads, 0, true);

        SaveWorkbook(package, outputFile);
    }

    private static async Task AnalyzeRecursiveAsync(string username, ExcelWorksheet sheet, int recursionLimit, int maxThreads, int depth = 0, bool firstEntry = false)
    {
        if (depth > recursionLimit || (VisitedUsers.ContainsKey(username) && !firstEntry))
            return;

        VisitedUsers.TryAdd(username, true);

        // var profile = await FetchProfileAsync(username);
        Console.WriteLine($"Parsing user {username}");

        if (!firstEntry)
        {
            var repos = await FetchReposAsync(username);
            var yearRepos = repos
                .Where(repo => MatchesYearRepoPattern(repo.Name))
                .Select(repo => new { repo.Name, repo.HtmlUrl })
                .ToList();

            if (yearRepos.Count > 0)
            {
                lock (LockObject)
                {
                    foreach (var repo in yearRepos)
                    {
                        WritedRows++;
                        sheet.Cells[sheet.Dimension.End.Row + 1, 1].Value = username;
                        sheet.Cells[sheet.Dimension.End.Row, 2].Value = repo.Name;
                        sheet.Cells[sheet.Dimension.End.Row, 3].Value = repo.HtmlUrl;
                    }
                }
            }
        }

        var following = await FetchFollowingAsync(username);
        var tasks = following
            .Where(f => f.Login != null && !VisitedUsers.ContainsKey(f.Login))
            .Select(f => AnalyzeRecursiveAsync(f.Login, sheet, recursionLimit, maxThreads, depth + 1));

        await Task.WhenAll(tasks);
    }

    private static async Task<List<Repository>> FetchReposAsync(string username)
    {
        var url = $"https://api.github.com/users/{username}/repos";
        return await FetchFromGitHubAsync<List<Repository>>(url);
    }

    private static async Task<UserProfile> FetchProfileAsync(string username)
    {
        var url = $"https://api.github.com/users/{username}";
        return await FetchFromGitHubAsync<UserProfile>(url);
    }

    private static async Task<List<UserProfile>> FetchFollowingAsync(string username)
    {
        var url = $"https://api.github.com/users/{username}/following";
        return await FetchFromGitHubAsync<List<UserProfile>>(url);
    }

    private static async Task<T> FetchFromGitHubAsync<T>(string url)
    {
        while (true)
        {
            await ApiSemaphore.WaitAsync();
            try
            {
                using var request = new HttpRequestMessage(HttpMethod.Get, url);

                // Добавление заголовков
                request.Headers.Add("User-Agent", "GitHubAnalyzer/1.0");
                request.Headers.Add("Authorization", $"token {Tokens[CurrentTokenIndex]}");

                var response = await HttpClient.SendAsync(request);

                if (response.IsSuccessStatusCode)
                {
                    var content = await response.Content.ReadAsStringAsync();
                    return System.Text.Json.JsonSerializer.Deserialize<T>(content);
                }

                // Если превышен лимит, переключаем токен
                if (response.StatusCode == System.Net.HttpStatusCode.Forbidden)
                {
                    Console.WriteLine("Rate limit exceeded or unauthorized. Switching token...");
                    SwitchToken();
                }
                else
                {
                    // Логирование ошибки для других статусов
                    Console.WriteLine($"Request to {url} failed with status {response.StatusCode}: {await response.Content.ReadAsStringAsync()}");
                    return default;
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error fetching data from {url}: {ex.Message}");
                return default;
            }
            finally
            {
                ApiSemaphore.Release();
            }
        }
    }

    private static void SwitchToken()
    {
        lock (LockObject)
        {
            CurrentTokenIndex = (CurrentTokenIndex + 1) % Tokens.Count;
            if (CurrentTokenIndex == 0)
                throw new Exception("All tokens have reached the rate limit.");
        }
    }

    private static void AdjustColumnWidths(ExcelWorksheet sheet)
    {
        for (var col = 1; col <= sheet.Dimension.Columns; col++)
        {
            sheet.Column(col).AutoFit(10, 100);
            sheet.Column(col).Width += 2;
        }
    }

    private static void SaveWorkbook(ExcelPackage package, string outputFile)
    {
        try
        {
            var sheet = package.Workbook.Worksheets["GitHub Analysis"];
            AdjustColumnWidths(sheet);
        }
        catch { };

        try
        {
            if (File.Exists(outputFile))
            {
                if (WritedRows == 0)
                {
                    Console.WriteLine("No new data to write.");
                    return;
                }

                Console.Write($"'{outputFile}' already exists. Do you want to override it (y/n)?: ");
                string? yn = Console.ReadLine();
                if (yn == "y")
                {
                    File.Delete(outputFile);
                }
            }
            package.SaveAs(new FileInfo(outputFile));
            Console.WriteLine($"Data saved to {outputFile}");
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Failed to save workbook: {ex.Message}");
        }
    }

    private static bool MatchesYearRepoPattern(string repoName)
    {
        return Regex.IsMatch(repoName, @"^[a-zA-Z]{3}-\d{4}$") && !repoName.ToLower().StartsWith("aoc-", StringComparison.OrdinalIgnoreCase);
    }

    public class UserProfile
    {
        [JsonPropertyName("login")]
        public string Login { get; set; }
    }

    public class Repository
    {
        [JsonPropertyName("name")]
        public string Name { get; set; }
        [JsonPropertyName("html_url")]
        public string HtmlUrl { get; set; }
    }
}
