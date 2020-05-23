using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace OSLab3
{
    public class System
    {
        Queue<CustomThread> checkQueue = new Queue<CustomThread>();
        Queue<CustomThread> allThreads = new Queue<CustomThread>();
        CustomThread start = new CustomThread(27, 1); //перша заявка
        int globalTime, counter;
        int maxSum = 100; //максимальна кылькысть заявок 

        public void Execute(int a)
        {
            checkQueue.Enqueue(start);
            while (checkQueue.Count > 0) 
            { 
                checkQueue = new Queue<CustomThread>(checkQueue.OrderBy(c => c.Priority));
                CustomThread current = checkQueue.Dequeue().DeepClone();
                if (allThreads.Count <= maxSum)
                {
                    int time = a;//GenerateTime();
                    CustomThread pretendent = GenerateThread().DeepClone();

                    if (pretendent.Priority < current.Priority)
                    {
                        globalTime += time;
                        checkQueue.Enqueue(current);
                        checkQueue.Enqueue(pretendent);
                    }
                    else
                    {
                        globalTime += current.Time;
                        checkQueue.Enqueue(pretendent);
                        allThreads.Enqueue(current);

                    }
                }
                else
                {
                    globalTime += current.Time;
                    
                }
            }
            Console.WriteLine(globalTime/allThreads.Count);
            // Console.WriteLine(counter); ;
            //foreach (var item in allThreads)
            //{
            //    Console.Write(item.Priority );
            //    Console.WriteLine();
            //}
        }





        public CustomThread GenerateThread()
        {
            Random rnd = new Random();
            int priority =  rnd.Next(1, 10);
            int time = rnd.Next(1,30);
            CustomThread thread = new CustomThread(time, priority);
            return thread;
        }
        public int GenerateTime()
        {
            Random rnd = new Random();
            int time = rnd.Next(1, 10);
            return time;
        }
    }
}
