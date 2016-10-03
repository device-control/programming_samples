using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ServiceState.State
{
    public class TestState : Common.State
    {
        protected TestState(string name) : base(name)
        {
        }

        public virtual void EventA(Common.Service service, Common.MQEvent ev)
        {
            // 何もしない
        }

        public virtual void EventB(Common.Service service, Common.MQEvent ev)
        {
            // 何もしない
        }

        public virtual void EventC(Common.Service service, Common.MQEvent ev)
        {
            // 何もしない
        }

        public override void HandleEvent(Common.Service service, Common.MQEvent ev)
        {
            switch (ev.Name)
            {
                case "EventA":
                    EventA(service, ev);
                    break;
                case "EventB":
                    EventB(service, ev);
                    break;
                case "EventC":
                    EventC(service, ev);
                    break;
                default:
                    break;
            }
        }
    }
}
