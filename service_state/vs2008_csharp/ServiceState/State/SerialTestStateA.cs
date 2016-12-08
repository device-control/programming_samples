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
            ((Service.SerialTestService)service).SendMessage(1,System.Text.Encoding.ASCII.GetBytes("{ \"eventName\" : \"EventB\" }\0"));
            Console.Write("Serial EventA\n");
            System.Threading.Thread.Sleep(1 * 1000);
        }
        
        public override void EventB(Common.Service service, Common.Event ev)
        {
            ((Service.SerialTestService)service).SendMessage(0,System.Text.Encoding.ASCII.GetBytes("{ \"eventName\" : \"EventC\" }\0"));
            Console.Write("Serial EventB\n");
            System.Threading.Thread.Sleep(1 * 1000);
        }

        public override void EventC(Common.Service service, Common.Event ev)
        {
            ((Service.SerialTestService)service).SendMessage(1,System.Text.Encoding.ASCII.GetBytes("{ \"eventName\" : \"EventD\" }\0"));
            Console.Write("Serial EventC\n");
            System.Threading.Thread.Sleep(1 * 1000);
        }
        
        public override void EventD(Common.Service service, Common.Event ev)
        {
            ((Service.SerialTestService)service).SendMessage(0,System.Text.Encoding.ASCII.GetBytes("{ \"eventName\" : \"EventA\" }\0"));
            Console.Write("Serial EventD\n");
            System.Threading.Thread.Sleep(1 * 1000);
        }

    }
}
