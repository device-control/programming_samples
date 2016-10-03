using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ServiceState.State
{
    class TestStateC : TestState
    {
        public TestStateC() : base("TestStateC")
        {
        }

        /*
        public override void EventA(common.Service service, common.MQEvent ev)
        {
            // 何もしない
        }

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
