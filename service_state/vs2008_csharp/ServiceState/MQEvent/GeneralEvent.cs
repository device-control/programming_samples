using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ServiceState.MQEvent
{
	public class GeneralEvent : Common.MQEvent
	{
        public int Number { get; set;}

        public GeneralEvent(string name) : base(name)
        {
        }

        public GeneralEvent()
        {
        }

	}
}
