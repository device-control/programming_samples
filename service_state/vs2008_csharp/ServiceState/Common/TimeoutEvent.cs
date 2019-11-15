using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ServiceState.Common
{
    public class TimeoutEvent: Event
    {
        public string TimerName { get; set; }
        public int SequenceNumber { get; set; }
        public TimeoutEvent(string name, string timerName, int sequenceNumber) : base(name)
        {
            this.Name = name;
            this.TimerName = timerName;
            this.SequenceNumber = sequenceNumber;
        }
   }
}
