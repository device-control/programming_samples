using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ServiceState.Service
{
    using IPConfig = Dictionary<string, string>;

    class SerialTestService : Common.Service, Common.IStreamObserver
    {
        private Common.SerialStream[] m_serialStream = {null, null};
        public SerialTestService() : base("TestService")
        {
            AddState(new State.SerialTestStateA());
            StartStateName = "TestStateA";

            for (int i = 1; i < 2; i++)
            {
                IPConfig ipconfig = new IPConfig();
                ipconfig["STREAM_NAME"] = "test_serial_stream0";
                if (i == 0)
                {
                    ipconfig["PORT_NAME"] = "COM5";
                }
                else
                {
                    ipconfig["PORT_NAME"] = "COM6";
                }
                m_serialStream[i] = new Common.SerialStream(ipconfig);
                m_serialStream[i].AddObserver(this);
                m_serialStream[i].Open();
            }
        }

        ~SerialTestService()
        {
            Dispose(false);
        }

        protected override void Dispose(bool disposing)
        {
            base.Dispose(disposing);
            m_serialStream[0].Close();
            m_serialStream[1].Close();
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

        public bool SendMessage(int index, byte[] bytes)
        {
            if (null == m_serialStream[index])
            {
                return false;
            }
            // byte[] bytes = System.Text.Encoding.ASCII.GetBytes("test_message")
            m_serialStream[index].Write(bytes);
            return true;
        }
    }
}
