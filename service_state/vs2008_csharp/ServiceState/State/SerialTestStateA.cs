using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ServiceState.State
{
    class SerialTestStateA : TestState
    {
        public SerialTestStateA() : base("SerialTestStateA")
        {
        }

        public override void EventA(Common.Service service, Common.Event ev)
        {
            ((Service.SerialTestService)service).SendMessage(1,System.Text.Encoding.ASCII.GetBytes("EventB"));
            Console.Write("Serial EventA\n");
            System.Threading.Thread.Sleep(1 * 1000);
        }
        
        public override void EventB(Common.Service service, Common.Event ev)
        {
            ((Service.SerialTestService)service).SendMessage(0,System.Text.Encoding.ASCII.GetBytes("EventC"));
            Console.Write("Serial EventB\n");
            System.Threading.Thread.Sleep(1 * 1000);
        }

        public override void EventC(Common.Service service, Common.Event ev)
        {
            ((Service.SerialTestService)service).SendMessage(1,System.Text.Encoding.ASCII.GetBytes("EventD"));
            Console.Write("Serial EventC\n");
            System.Threading.Thread.Sleep(1 * 1000);
        }
        
        public override void EventD(Common.Service service, Common.Event ev)
        {
            ((Service.SerialTestService)service).SendMessage(0,System.Text.Encoding.ASCII.GetBytes("EventA"));
            Console.Write("Serial EventD\n");
            System.Threading.Thread.Sleep(1 * 1000);
        }

    }
}
