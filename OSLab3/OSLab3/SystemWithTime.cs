using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;

namespace OSLab3
{
    class SystemWithTime
    {
        Queue<CustomThreadWithTime> checkQueue = new Queue<CustomThreadWithTime>();
        Queue<CustomThreadWithTime> allThreads = new Queue<CustomThreadWithTime>();
        CustomThreadWithTime start = new CustomThreadWithTime(27, 1, 0); //перша заявка
        int globalTime, counter;
        int maxSum = 1000; //максимальна кылькысть заявок 

        public void Execute(int a)
        {
            checkQueue.Enqueue(start);
            allThreads.Enqueue(start);
            while (checkQueue.Count > 0)
            {
                checkQueue = new Queue<CustomThreadWithTime>(checkQueue.OrderBy(c => c.Priority));
                CustomThreadWithTime current = checkQueue.Dequeue();
                if (allThreads.Count < maxSum)
                {
                    int time = a;//GenerateTime();
                    CustomThreadWithTime pretendent = GenerateThread();
                    if (pretendent.WaitingTime == 0)
                    {

                        pretendent.WaitingTime = globalTime;
                    }
                    if (pretendent.Priority < current.Priority)
                    {
                        globalTime += time;
                        current.WaitingTime+=time;
                        checkQueue.Enqueue(current);
                        checkQueue.Enqueue(pretendent);
                    }
                    else
                    {
                        globalTime += current.Time;
                        counter++;
                        checkQueue.Enqueue(pretendent);
                        current.WaitingTime = globalTime - current.WaitingTime;
                        allThreads.Enqueue(current);

                    }
                }
                else
                {
                    globalTime += current.Time;
                    counter++;
                }
            }
            Console.WriteLine(globalTime / counter);
            // Console.WriteLine(counter); ;
            List<ResultLine> res = allThreads.
                GroupBy(l => l.Priority)
                .Select(cl => new ResultLine
                {
                    priority = cl.First().Priority,
                    Sum = cl.Sum(c => c.WaitingTime),
                    Counted = cl.Count(),
                }).OrderBy(cl => cl.priority).ToList();
            foreach (var resultLine in res)
            {
                Console.Write(resultLine.priority);
                Console.WriteLine();
            }
            foreach (var resultLine in res)
            {
                Console.Write(resultLine.Counted);
                Console.WriteLine();
            }
            foreach (var resultLine in res)
            {
                Console.Write(resultLine.Sum);
                Console.WriteLine();
            }
            foreach (var resultLine in res)
            {
                Console.Write(resultLine.priority + " " + resultLine.Counted + " " + resultLine.Sum);
                Console.WriteLine();
            }
            /* Console.WriteLine("ALL");
             foreach (var item in allThreads)
             {
                 Console.Write(item.Priority + " "+item.WaitingTime);
                 Console.WriteLine();
             }
             */
        }





        public CustomThreadWithTime GenerateThread()
        {
            Random rnd = new Random();
            int priority = rnd.Next(1,30);
            int time = rnd.Next(1, 50);
            CustomThreadWithTime thread = new CustomThreadWithTime(time, priority, 0);

            return thread;
        }
        public int GenerateTime()
        {
            Random rnd = new Random();
            int time = rnd.Next(1, 10);
            return time;
        }
    }
    public class ResultLine
    {

        public ResultLine() { }

       // public string ProductName { get; set; }
        public int priority { get; set; }
        public int Sum { get; set; }
        public int Counted { get; set; }
    }
}