using System;
using System.Collections.Generic;
using System.Text;

namespace OSLab3
{
    [Serializable]
    class CustomThreadWithTime
    {
        public int Time { get; set; }
        public int Priority { get; set; }
        public int WaitingTime { get; set; }
        public CustomThreadWithTime(int t, int p,int wt)
        {
            this.Time = t;
            this.Priority = p;
            this.WaitingTime = wt;
        }
    }
}
