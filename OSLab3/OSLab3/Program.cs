using System;
using System.Threading;

namespace OSLab3
{
    class Program
    {
        static void Main(string[] args)
        {
           
            for (int i = 0; i < 10; i++)
            {
                System s = new System();
                Console.Write(i + " ");
                s.Execute(i);
            }
            Console.WriteLine("PART2");
            SystemWithTime st = new SystemWithTime();
            st.Execute(1);

            Console.ReadLine();
        }
    }
}
