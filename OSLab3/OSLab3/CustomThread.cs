using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection.PortableExecutable;
using System.Runtime.Serialization.Formatters.Binary;
using System.Text;

namespace OSLab3
{
    [Serializable]
    public class CustomThread
    {
        public int Time { get; set; }
        public int Priority { get; set; }
        public CustomThread(int t, int p)
        {
            this.Time = t;
            this.Priority = p;
        }
    }
    public static class ExtensionMethods
    {
        // Deep clone
        public static T DeepClone<T>(this T a)
        {
            using (MemoryStream stream = new MemoryStream())
            {
                BinaryFormatter formatter = new BinaryFormatter();
                formatter.Serialize(stream, a);
                stream.Position = 0;
                return (T)formatter.Deserialize(stream);
            }
        }
    }
}
