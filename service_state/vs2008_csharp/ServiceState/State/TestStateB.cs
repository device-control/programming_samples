using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ServiceState.State
{
    class TestStateB : TestState
    {
        public TestStateB() : base("TestStateB")
        {
        }

        /*
        public override void EventA(common.Service service, common.MQEvent ev)
        {
            // 何もしない
        }
         */


        public override void EventB(Common.Service service, Common.MQEvent ev)
        {
            ((Service.TestService)service).SendMessage(System.Text.Encoding.ASCII.GetBytes("exec TestStateB::EventB"));
            service.ChangeState("TestStateC");
        }

        /*
        public override void EventC(common.Service service, common.MQEvent ev)
        {
            // 何もしない
        }
         */

    }
}
