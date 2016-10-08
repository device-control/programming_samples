using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ServiceState.State
{
    class TCPTestStateA : TestState
    {
        public TCPTestStateA() : base("TCPTestStateA")
        {
        }

        public override void EventA(Common.Service service, Common.Event ev)
        {
            ((Service.TCPTestService)service).SendMessageFromClient(System.Text.Encoding.ASCII.GetBytes("{ \"eventName\" : \"EventB\" }\0"));
            Console.Write("TCP EventA\n");
            System.Threading.Thread.Sleep(1 * 1000);
        }
        
        public override void EventB(Common.Service service, Common.Event ev)
        {
            ((Service.TCPTestService)service).SendMessageFromServer(System.Text.Encoding.ASCII.GetBytes("{ \"eventName\" : \"EventC\" }\0"));
            Console.Write("TCP EventB\n");
            System.Threading.Thread.Sleep(1 * 1000);
        }

        public override void EventC(Common.Service service, Common.Event ev)
        {
            ((Service.TCPTestService)service).SendMessageFromClient(System.Text.Encoding.ASCII.GetBytes("{ \"eventName\" : \"EventD\" }\0"));
            Console.Write("TCP EventC\n");
            System.Threading.Thread.Sleep(1 * 1000);
        }
        
        public override void EventD(Common.Service service, Common.Event ev)
        {
            ((Service.TCPTestService)service).SendMessageFromServer(System.Text.Encoding.ASCII.GetBytes("{ \"eventName\" : \"EventA\" }\0"));
            Console.Write("TCP EventD\n");
            System.Threading.Thread.Sleep(1 * 1000);
        }

    }
}
