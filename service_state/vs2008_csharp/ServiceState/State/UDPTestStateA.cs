using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ServiceState.State
{
    class UDPTestStateA : TestState
    {
        public UDPTestStateA() : base("TestStateA")
        {
        }

        public override void EventA(Common.Service service, Common.Event ev)
        {
            System.Threading.Thread.Sleep(1000 * 1);
            ((Service.UDPTestService)service).SendMessage(System.Text.Encoding.ASCII.GetBytes("EventB"));
            service.ChangeState("TestStateB");
        }
    }
}
