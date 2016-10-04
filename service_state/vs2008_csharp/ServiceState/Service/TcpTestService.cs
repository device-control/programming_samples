using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Json;

// JSON 使い方なら以下を"参照の追加"で登録すること
// ・System.Runtime.Serialization
// ・System.ServiceModel
// ・System.Servicemodel.Web

namespace ServiceState.Service
{
    using IPConfig = Dictionary<string, string>;

    

    class TCPTestService : Common.Service, Common.IStreamObserver
    {
        private Common.TCPServerStream tcpServerStream = null;
        private Common.TCPClientStream tcpClientStream = null;
        Dictionary<string, Byte[]> buffers = new Dictionary<string, Byte[]>();
        Dictionary<string, int> buffersIndex = new Dictionary<string, int>();

        [DataContract]
        class TCPMessage
        {
            [DataMember]
            public string eventName;
        }
        
        public TCPTestService() : base("TCPTestService")
        {
            buffers["test_tcp_server"] = new Byte[64*1024];
            buffers["test_tcp_client"] = new Byte[64*1024];
            buffersIndex["test_tcp_server"] = 0;
            buffersIndex["test_tcp_client"] = 0;

            // 本サービスで取り扱う（受信する）イベントを登録する
            SetReceiveMQEvents(new Type[] { typeof(MQEvent.GeneralEvent) });

            AddState(new State.TCPTestStateA());
            StartStateName = "TCPTestStateA";

            IPConfig server_ipconfig = new IPConfig();
            server_ipconfig["STREAM_NAME"] = "test_tcp_server"; // *ストリーム名
            server_ipconfig["RECEIVE_IP"] = "127.0.0.1"; // 受信ＩＰ(無指定の場合、localhost)
            server_ipconfig["RECEIVE_PORT"] = "4989"; // 受信ポート(無指定の場合、受信しない)
            tcpServerStream = new Common.TCPServerStream(server_ipconfig);
            tcpServerStream.AddObserver(this);
            tcpServerStream.Open();

            IPConfig client_ipconfig = new IPConfig();
            client_ipconfig["STREAM_NAME"] = "test_tcp_client"; // *ストリーム名
            client_ipconfig["SEND_IP"] = "127.0.0.1"; // 受信ＩＰ(無指定の場合、localhost)
            client_ipconfig["SEND_PORT"] = "4989"; // 受信ポート(無指定の場合、受信しない)
            tcpClientStream = new Common.TCPClientStream(client_ipconfig);
            tcpClientStream.AddObserver(this);

            System.Threading.Thread.Sleep(1*1000);
            tcpClientStream.Open();
        }

        ~TCPTestService()
        {
            Dispose(false);
        }

        protected override void Dispose(bool disposing)
        {
            base.Dispose(disposing);
            tcpClientStream.Close();
        }

        void Common.IStreamObserver.StatusChanged(Common.Stream stream, Common.Stream.Status status)
        {
            Console.WriteLine("status change {0}", status);
        }

        void Common.IStreamObserver.MessageReceived(Common.Stream stream, Byte[] bytes)
        {
            string name = stream.GetConfig("STREAM_NAME");

            Buffer.BlockCopy(bytes, 0, buffers[name], buffersIndex[name], bytes.Length);
            int startIndex = buffersIndex[name];
            buffersIndex[name] += bytes.Length;
            int endIndex = buffersIndex[name];
            int index = startIndex;
            for (; index < buffersIndex[name]; index++)
            {
                if (0 == buffers[name][index])
                {
                    break;
                }
            }
            if (index == buffersIndex[name])
            {
                // 続きがある
                return;
            }
            Byte[] one = new Byte[index];
            Buffer.BlockCopy(buffers[name], 0, one, 0, index);
            if ((index + 1) != buffersIndex[name])
            {
                Buffer.BlockCopy(buffers[name], 0, buffers[name], index + 1, buffersIndex[name] - (index + 1));
            }
            buffersIndex[name] = 0;

            var serializer = new DataContractJsonSerializer(typeof(TCPMessage));
            using (var ms = new System.IO.MemoryStream(one))
            {
                var data = (TCPMessage)serializer.ReadObject(ms);
                Console.WriteLine(data.eventName);
                AddEvent(new MQEvent.GeneralEvent(data.eventName));
            }
            //string oneString = System.Text.Encoding.ASCII.GetString(one);
            //AddEvent(new MQEvent.GeneralEvent(oneString));
            // Console.WriteLine("message received[{0}]", System.Text.Encoding.ASCII.GetString(bytes));
        }

        public bool SendMessageFromClient(byte[] bytes)
        {
            if (null == tcpClientStream)
            {
                return false;
            }
            tcpClientStream.Write(bytes);
            return true;
        }
        public bool SendMessageFromServer(byte[] bytes)
        {
            if (null == tcpServerStream)
            {
                return false;
            }
            tcpServerStream.Write(bytes);
            return true;
        }
    }
}
