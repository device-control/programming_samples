﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ServiceState.State
{
    class UDPTestStateC : TestState
    {
        public UDPTestStateC() : base("TestStateC")
        {
        }

        public override void EventC(Common.Service service, Common.Event ev)
        {
            System.Threading.Thread.Sleep(1000 * 1);
            ((Service.UDPTestService)service).SendMessage(System.Text.Encoding.ASCII.GetBytes("EventA"));
            service.ChangeState("TestStateA");
        }
    }
}
