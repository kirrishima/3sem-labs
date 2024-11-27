using OOP_Lab15;
using System.Diagnostics;

namespace OOP_Lab15
{
    class Program
    {
        static async Task Main(string[] args)
        {
            // 1-2
            //CancableTask.Task();

            //3-4
            //ContinuationTasks.Tasks();

            //5
            //ParralelVsSequential.Compare();

            //6
            //ParallelInvoke.Test();

            Warehouse warehouse = new Warehouse();
            await warehouse.Start();
        }
    }
}
