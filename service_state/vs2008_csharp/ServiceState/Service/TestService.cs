using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ServiceState.Service
{
    using IPConfig = Dictionary<string, string>;

    class TestService : Common.Service, Common.IStreamObserver
    {
        private Common.UDPStream udpStream = null;
        public TestService() : base("TestService")
        {
            AddState(new State.TestStateA());
            AddState(new State.TestStateB());
            AddState(new State.TestStateC());
            StartStateName = "TestStateA";

            
            IPConfig ipconfig = new IPConfig();
            ipconfig["STREAM_NAME"] = "test_udp_stream"; // *ストリーム名
            ipconfig["SEND_IP"] = "127.0.0.1"; // 送信先ＩＰ(無指定の場合、localhost)
            ipconfig["SEND_PORT"] = "20001"; // 送信先ポート(無指定の場合、送信しない)
            ipconfig["RECEIVE_IP"] = "127.0.0.1"; // 受信ＩＰ(無指定の場合、localhost)
            ipconfig["RECEIVE_PORT"] = "20001"; // 受信ポート(無指定の場合、受信しない)
            udpStream = new Common.UDPStream(ipconfig);
            udpStream.AddObserver(this);
            udpStream.Open();
            
        }

        ~TestService()
        {
            Dispose(false);
        }

        protected override void Dispose(bool disposing)
        {
            base.Dispose(disposing);
            udpStream.Close();
        }

        void Common.IStreamObserver.StatusChanged(Common.Stream stream, Common.Stream.Status status)
        {
            Console.WriteLine("status change {0} {1}", stream.GetConfig("STREAM_NAME"), status);
        }

        void Common.IStreamObserver.MessageReceived(Common.Stream stream, Byte[] bytes)
        {
            Console.WriteLine("message received {0} [{1}]", stream.GetConfig("STREAM_NAME"), System.Text.Encoding.ASCII.GetString(bytes));
            string eventName = System.Text.Encoding.ASCII.GetString(bytes);
            AddEvent(new Common.Event(eventName));
        }

        public bool SendMessage(byte[] bytes)
        {
            if (null == udpStream)
            {
                return false;
            }
            // byte[] bytes = System.Text.Encoding.ASCII.GetBytes("test_message")
            udpStream.Write(bytes);
            return true;
        }
    }
}
