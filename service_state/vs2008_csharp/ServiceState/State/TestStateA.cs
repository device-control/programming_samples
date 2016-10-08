using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ServiceState.State
{
    class TestStateA : TestState
    {
        public TestStateA() : base("TestStateA")
        {
        }

        public override void EventA(Common.Service service, Common.Event ev)
        {
            ((Service.TestService)service).SendMessage(System.Text.Encoding.ASCII.GetBytes("exec TestStateA::EventA"));
            service.ChangeState("TestStateB");
        }
    }
}
