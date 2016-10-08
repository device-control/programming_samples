using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ServiceState.Common
{
    public class Event
    {
        public string Name { get; set; }
        public string Body { get; set; }
        
        protected Event(string name, string body)
        {
            Name = name;
            Body = body;
        }

        public Event(string name)
        {
            Name = name;
            Body = "";
        }

    }
}
