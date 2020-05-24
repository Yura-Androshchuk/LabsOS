using System;
using System.Diagnostics;

namespace ConsoleApp1
{
    class Program
    {
        static void Main(string[] args)
        {
            Stopwatch stopwatch = new Stopwatch();
            stopwatch.Start();
            int[,,] b = new int[10, 10, 10];
            //int res = 0;
            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    for (int k = 0; k < 10; k++)
                    {
                        b[k, j, i]++;
                    }
                }
            }
            stopwatch.Stop();
            TimeSpan timeSpan = TimeSpan.FromTicks(stopwatch.ElapsedTicks);
            string timeInterval = timeSpan.ToString();
            Console.WriteLine("Time spent: " + timeInterval);
            Console.WriteLine();
        }
    }
}
