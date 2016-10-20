using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ServiceState.Common
{
    public class Event
    {
        public string Name { get; set; }
        public byte[] Body { get; set; }
        
        public Event(string name, string body)
        {
            Name = name;
            Body = System.Text.Encoding.UTF8.GetBytes(body); // UTF8 以外考慮しない
        }

        public Event(string name, byte[] body)
        {
            Name = name;
            Body = body;
        }

        public Event(string name)
        {
            Name = name;
            Body = null;
        }

    }
}
