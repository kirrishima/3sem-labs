using System.Diagnostics;

namespace OOP_Lab14
{
    class Program
    {
        private static string Truncate(string s, int maxSize)
        {
            s = s.Trim();

            if (s.Length >= maxSize - 3)
            {
                s = s.Substring(0, maxSize - 3);
                s += "...";
            }

            return s;
        }

        static void Main(string[] args)
        {
            var currentProcesses = Process.GetProcesses();

            //1
            /*int fails = 0;

            Console.WriteLine($"{"ID",-6} {"Process name",15} {"Priority",22} {"Start time",22} {"Current state",21} {"CPU time",16}");
            foreach (var process in currentProcesses)
            {
                try
                {
                    Console.WriteLine($"{process.Id,-6} {Truncate(process.ProcessName, 26),-30} {process.PriorityClass,-15} {process.StartTime,-20} Has exited: {process.HasExited} {process.TotalProcessorTime,18}");
                }
                catch (Exception ex)
                {
                    fails++;
                }
            }

            Console.WriteLine($"\nFailed: {fails}");*/

            Console.WriteLine();
            //2

            var domain = AppDomain.CurrentDomain;
            Console.WriteLine(domain.FriendlyName);
            Console.WriteLine();
            foreach (var item in domain.GetAssemblies())
            {
                Console.WriteLine(item);
            }
            Console.WriteLine();

            Console.WriteLine(domain.SetupInformation.TargetFrameworkName);
            Console.WriteLine(domain.SetupInformation.ApplicationBase);

            // устарело
            /* var newDomain = AppDomain.CreateDomain("52");
             string assemblyPath = typeof(Program).Assembly.Location;
             newDomain.Load(AssemblyName.GetAssemblyName(assemblyPath));
             AppDomain.Unload(newDomain);*/

            //3 

            Console.Clear();
            /*PrimeNumbers.StartShowcase();*/


            // 4
            OddEvenNumbers.Showcase();


            //5
            var cb = new TimerCallback(Callback);
            Timer timer = new Timer(cb, null, 0, 1000);

            Console.ReadLine();
        }

        public static void Callback(object? obj)
        {
            Console.WriteLine(DateTime.Now);
        }
    }
}