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

        public override void EventA(Common.Service service, Common.MQEvent ev)
        {
            ((Service.TestService)service).SendMessage(System.Text.Encoding.ASCII.GetBytes("exec TestStateA::EventA"));
            service.ChangeState("TestStateB");
        }
        
        /*
        public override void EventB(common.Service service, common.MQEvent ev)
        {
            // 何もしない
        }

        public override void EventC(common.Service service, common.MQEvent ev)
        {
            // 何もしない
        }
         */

    }
}
