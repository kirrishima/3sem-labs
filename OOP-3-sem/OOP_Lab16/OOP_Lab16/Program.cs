using OfficeOpenXml;
using OfficeOpenXml.Packaging.Ionic.Zip;
using OfficeOpenXml.Style;
using System.Collections.Concurrent;
using System.Drawing;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Text.RegularExpressions;

internal class Program
{
    private static readonly HttpClient HttpClient;
    private static SemaphoreSlim ApiSemaphore;
    private static List<string> Tokens;
    private static int CurrentTokenIndex = 0;
    private static ConcurrentDictionary<string, bool> VisitedUsers;
    private static readonly string VisistedFilepath = "VisitedUsers.json";
    private static readonly string UsersBlacklistFilepath = "UsersBlacklist.json";
    private static ConcurrentDictionary<string, bool> UsersBlacklist;
    private static SemaphoreSlim ThreadLimiter;
    private static int MaxThreads;
    private static int WritedRows = 0;
    private static int Scanned = 0;
    private static readonly object excelLock = new();
    private static object queueLock = new();
    private static object SwitchTokenLock = new();
    private static bool Exited = false;
    private static ConcurrentQueue<(string Username, int Depth)> queue;
    private static ConcurrentQueue<(string username, int depth)> intermediateQueue = new();
    private static Task? backgroundTask;
    private static CancellationTokenSource cancellationTokenSource = new();

    static Program()
    {
        if (File.Exists("tokens.json"))
        {
            var jsonString = File.ReadAllText("tokens.json");
            var tokens = JsonSerializer.Deserialize<ApiTokens>(jsonString);
            Tokens = tokens?.Tokens ?? [];
        }
        else
        {
            throw new Exception("Could not find tokens.json");
        }

        HttpClient = new()
        {
            Timeout = TimeSpan.FromSeconds(5)
        };

        if (File.Exists(VisistedFilepath))
        {
            VisitedUsers = DeserializeConcurrentDictionaryAsync<string, bool>(VisistedFilepath).Result;
        }
        else
        {
            VisitedUsers = new();
        }

        if (File.Exists(UsersBlacklistFilepath))
        {
            UsersBlacklist = DeserializeConcurrentDictionaryAsync<string, bool>(UsersBlacklistFilepath).Result;
        }
        else
        {
            UsersBlacklist = new();
        }

        MaxThreads = Environment.ProcessorCount * 2;

        ThreadLimiter = new SemaphoreSlim(MaxThreads, MaxThreads); // Ограничение на 6 одновременно работающих потоков

        ApiSemaphore = new SemaphoreSlim(Math.Min(10, MaxThreads), Math.Min(10, MaxThreads)); // Max 10 concurrent requests

        queue = new();
    }

    private static async Task Main(string[] args)
    {
        ExcelPackage.LicenseContext = LicenseContext.NonCommercial;
        string[] startUsers = ["knvzzi", "Vikvillka", "miwuzo", "darlitaa", "mashassnvts", "mashassnvts", "vivsii"];
        var outputExcel = $"github_analysis_results_{DateTime.Now:yyyy-MM-dd_HH-mm-ss}.xlsx";

        await AnalyzeUsersWithQueueAsync(startUsers, outputExcel, recursionLimit: 10, maxThreads: 12);
        //await AnalyzeUserAsync(startUsers, outputExcel, recursionLimit: 10, maxThreads: 6);

        await SerializeConcurrentDictionaryAsync(VisitedUsers, VisistedFilepath);


        /*        List<Repository> list = [new Repository() { Name = "AISD2sem" }, new Repository() { Name = "KSIS2sem" }];

                Console.WriteLine(IsStudent(list));*/
    }

    private static async Task SerializeConcurrentDictionaryAsync<TK, TV>(ConcurrentDictionary<TK, TV> dictionary, string filePath) where TK : notnull
    {
        var serializedData = JsonSerializer.Serialize(dictionary);
        await File.WriteAllTextAsync(filePath, serializedData);
    }

    private static async Task<ConcurrentDictionary<TK, TV>> DeserializeConcurrentDictionaryAsync<TK, TV>(string filePath) where TK : notnull
    {
        if (!File.Exists(filePath))
        {
            return new ConcurrentDictionary<TK, TV>();
        }

        var serializedData = await File.ReadAllTextAsync(filePath);
        var dictionary = JsonSerializer.Deserialize<ConcurrentDictionary<TK, TV>>(serializedData);

        return dictionary ?? new ConcurrentDictionary<TK, TV>();
    }

    private static async Task AnalyzeUsersWithQueueAsync(string[] startUsers, string outputFile, int recursionLimit, int maxThreads)
    {
        using var package = new ExcelPackage();
        var sheet = package.Workbook.Worksheets.Add("GitHub Analysis");
        sheet.Cells[1, 1].Value = "User";
        sheet.Cells[1, 2].Value = "Repository Name";
        sheet.Cells[1, 3].Value = "Repository URL";

        // Применяем стиль к первой строке
        using (var range = sheet.Cells[1, 1, 1, 3])
        {
            range.Style.Font.Bold = true;
            range.Style.Font.Size = 14;
            range.Style.Font.Color.SetColor(Color.White);
            range.Style.Fill.PatternType = ExcelFillStyle.Solid;
            range.Style.Fill.BackgroundColor.SetColor(Color.DarkBlue);
            range.Style.HorizontalAlignment = ExcelHorizontalAlignment.Center;
            range.Style.VerticalAlignment = ExcelVerticalAlignment.Center;
        }

        Console.CancelKeyPress += (sender, eventArgs) =>
        {
            if (!Exited)
            {
                Environment.Exit(0);
                /*                Console.WriteLine("\n\nTermination signal received.");
                                Console.WriteLine("Saving table...");
                                SaveWorkbook(package, outputFile);
                                Console.WriteLine("Serializing visited users...");
                                SerializeConcurrentDictionaryAsync(VisitedUsers, VisistedFilepath).Wait();
                                SerializeConcurrentDictionaryAsync(UsersBlacklist, UsersBlacklistFilepath).Wait();*/
            }
        };

        AppDomain.CurrentDomain.ProcessExit += (sender, eventArgs) =>
        {
            if (!Exited)
            {
                Exited = true;
                Console.WriteLine("\n\nDomain process exiting entercepted.");
                Console.WriteLine("Saving table...");
                SaveWorkbook(package, outputFile);
                Console.WriteLine("Serializing visited users...");
                SerializeConcurrentDictionaryAsync(VisitedUsers, VisistedFilepath).Wait();
                SerializeConcurrentDictionaryAsync(UsersBlacklist, UsersBlacklistFilepath).Wait();
            }
        };

        try
        {
            // Подготовка очереди задач
            foreach (var user in startUsers)
            {
                queue.Enqueue((user, 0));
            }

            // Распределение токенов между потоками
            var tokenBuckets = DistributeTokens(Tokens, maxThreads);

            Console.WriteLine($"Using up to {tokenBuckets.Count} threads.\n");

            StartBackgroundTask(); // Запуск фоновой проверки

            var tasks = new List<Task>();
            for (int i = 0; i < tokenBuckets.Count; i++)
            {
                var threadTokens = tokenBuckets[i];
                tasks.Add(Task.Run(() => ProcessQueue(threadTokens, recursionLimit, sheet)));
            }

            Console.WriteLine($"Starting {tasks.Count} threads...");

            await Task.WhenAll(tasks);
        }
        catch { }

        finally
        {
            Exited = true;
            SaveWorkbook(package, outputFile);
        }
    }

    private static List<List<string>> DistributeTokens(List<string> tokens, int maxThreads)
    {
        var tokenBuckets = new List<List<string>>();

        int bucketCount = Math.Min(maxThreads, tokens.Count);
        int tokensPerBucket = tokens.Count / bucketCount;
        int extraTokens = tokens.Count % bucketCount;

        int index = 0;
        for (int i = 0; i < bucketCount; i++)
        {
            int bucketSize = tokensPerBucket + (extraTokens-- > 0 ? 1 : 0);
            var bucket = tokens.Skip(index).Take(bucketSize).ToList();
            tokenBuckets.Add(bucket);
            index += bucketSize;
        }

        return tokenBuckets;
    }

    // Запуск фоновой задачи
    private static void StartBackgroundTask()
    {
        backgroundTask = Task.Run(async () =>
        {
            while (!cancellationTokenSource.Token.IsCancellationRequested)
            {
                if (intermediateQueue.TryDequeue(out var item))
                {
                    var (username, depth) = item;
                    if (!VisitedUsers.ContainsKey(username))
                    {
                        queue.Enqueue(item);
                    }
                }
            }
        });
    }

    private static async Task ProcessQueue(List<string> tokens, int recursionLimit, ExcelWorksheet sheet)
    {
        int? ID = Task.CurrentId;

        int currentTokenIndex = 0;

        while (true)
        {
            while (queue.TryDequeue(out var item))
            {
                var (username, depth) = item;

                if (depth > recursionLimit || VisitedUsers.ContainsKey(username) || UsersBlacklist.ContainsKey(username))
                {
                    Console.WriteLine($"[Thread {ID}]: skip {username}");
                    continue;
                }

                VisitedUsers.TryAdd(username, true);
                var token = tokens[currentTokenIndex];

                try
                {
                    Scanned++;
                    if (!UsersBlacklist.ContainsKey(username))
                    {
                        Console.WriteLine($"[Thread {ID}]: Analyzing user {username}.");
                        var repos = await FetchReposAsync(username, token);

                        if (!IsStudent(repos))
                        {
                            Console.WriteLine($"[Thread {ID}]: not a student: {username}");
                            UsersBlacklist.TryAdd(username, true);
                        }
                        else
                        {
                            var yearRepos = repos?
                               .Where(repo => MatchesYearRepoPattern(repo?.Name))
                               .Select(repo => new { repo?.Name, repo?.HtmlUrl })
                               .ToList();

                            if (yearRepos != null && yearRepos.Any())
                            {
                                lock (excelLock)
                                {
                                    foreach (var repo in yearRepos)
                                    {
                                        var row = (sheet.Dimension?.End.Row ?? 1) + 1;
                                        sheet.Cells[row, 1].Value = username;
                                        sheet.Cells[row, 2].Value = repo.Name;
                                        sheet.Cells[row, 3].Hyperlink = new Uri(repo.HtmlUrl);
                                        sheet.Cells[row, 3].Value = repo.HtmlUrl;
                                        WritedRows++;
                                    }
                                }
                            }
                        }
                        var followers = await FetchFollowersAsync(username, token);
                        var following = await FetchFollowingAsync(username, token);

                        var newUsers = (followers ?? Enumerable.Empty<UserProfile>())
                            .Concat(following ?? Enumerable.Empty<UserProfile>()) // Объединяем списки
                            .Where(u => u.Login != null && !VisitedUsers.ContainsKey(u.Login)) // Фильтруем по условию
                            .Select(u => u.Login!) // Берем логины
                            .Distinct() // Убираем дубликаты логинов
                            .Select(login => (login, depth + 1)); // Преобразуем в результат

                        /*                      lock (queueLock)
                                              {
                                                  foreach (var user in newUsers)
                                                  {
                                                      if (!VisitedUsers.ContainsKey(user.login))
                                                      {
                                                          queue.Enqueue(user);
                                                      }
                                                  }
                                              }*/

                        foreach (var user in newUsers)
                        {
                            intermediateQueue.Enqueue(user); // Отправляем в промежуточную очередь
                        }
                    }

                }
                catch (Exception ex)
                {
                    currentTokenIndex = (currentTokenIndex + 1) % tokens.Count;
                    if (currentTokenIndex == 0)
                    {
                        Console.WriteLine($"[Thread {ID}]: Api Tokens ended. Exiting thread...");
                        return;
                    }
                    intermediateQueue.Enqueue((username, depth));
                    Console.WriteLine($"Error processing {username}: {ex.Message}");
                }
            }

            Console.WriteLine($"[Thread {ID}]: Queue is empty, waiting for 10s...");
            await Task.Delay(TimeSpan.FromSeconds(10));

            if (queue.Count == 0)
            {
                return;
            }
        }
    }

    private static async Task<List<Repository?>?> FetchReposAsync(string username, string token)
    {
        var url = $"https://api.github.com/users/{username}/repos";
        return await FetchFromGitHubAsync<List<Repository?>?>(url, token);
    }

    private static async Task<List<UserProfile?>?> FetchFollowersAsync(string username, string token)
    {
        var url = $"https://api.github.com/users/{username}/followers";
        return await FetchFromGitHubAsync<List<UserProfile?>?>(url, token);
    }

    private static async Task<List<UserProfile?>?> FetchFollowingAsync(string username, string token)
    {
        var url = $"https://api.github.com/users/{username}/following";
        return await FetchFromGitHubAsync<List<UserProfile?>?>(url, token);
    }

    private static async Task<T?> FetchFromGitHubAsync<T>(string url, string token)
    {
        using var request = new HttpRequestMessage(HttpMethod.Get, url);
        request.Headers.Add("User-Agent", "GitHubAnalyzer/1.0");
        request.Headers.Add("Authorization", $"token {token}");

        var response = await HttpClient.SendAsync(request);

        response.EnsureSuccessStatusCode();

        var content = await response.Content.ReadAsStringAsync();
        return JsonSerializer.Deserialize<T>(content);
    }

    private static void SwitchToken()
    {
        lock (excelLock)
        {
            CurrentTokenIndex = (CurrentTokenIndex + 1) % Tokens.Count;
            Task.Delay(TimeSpan.FromSeconds(2)).Wait();

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
            Console.WriteLine($"Scanned users: {Scanned}");
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

    private static bool IsStudent(List<Repository?>? repositories)
    {
        if (repositories == null || repositories.Count == 0)
            return false;

        // Список ключевых подстрок, характерных для студенческих репозиториев
        var studentKeywords = new HashSet<string>(StringComparer.OrdinalIgnoreCase)
        {
            "lab", "labs", "sem", "kpo", "oop", "term", "bstu", "exam",
            "tvims", "ksis", "trpi", "tpo", "aisd", "course", "student"
        };

        // Проверяем каждое название репозитория
        foreach (var repo in repositories)
        {
            if (repo?.Name == null)
                continue;

            // Проверяем, содержит ли название какую-либо из подстрок
            foreach (var keyword in studentKeywords)
            {
                if (repo.Name.Contains(keyword, StringComparison.OrdinalIgnoreCase))
                    return true;
            }
        }

        return false;
    }

    private static bool MatchesYearRepoPattern(string? repoName)
    {
        if (repoName == null)
            return false;

        return Regex.IsMatch(repoName, @"^[a-zA-Z]{3}-\d{4}$") && !repoName.StartsWith("aoc-", StringComparison.OrdinalIgnoreCase);
    }

    public class UserProfile
    {
        [JsonPropertyName("login")]
        public string? Login { get; set; }
    }

    public class Repository
    {
        [JsonPropertyName("name")]
        public string? Name { get; set; }

        [JsonPropertyName("html_url")]
        public string? HtmlUrl { get; set; }
    }

    public class ApiTokens
    {
        [JsonPropertyName("tokens")]
        public List<string>? Tokens { get; set; }
    }
}