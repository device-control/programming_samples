using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace ServiceState.Common
{
    using IPConfig = Dictionary<string, string>;

    class UDPStream : Stream
    {
        /* 
         * TODO: 以下のパラメータが存在するかの確認を行うのはStreamManagerで行い
         *       パラメータが正常な場合、本コンストラクタを呼び出すようにしたい。
         * STREAM_NAME  : *ストリーム名
         * SEND_IP      :  送信先ＩＰ(無指定の場合、localhost)
         * SEND_PORT    :  送信先ポート(無指定の場合、送信しない)
         * RECEIVE_IP   :  受信ＩＰ(無指定の場合、localhost)
         * RECEIVE_PORT :  受信ポート(無指定の場合、受信しない)
         */
        private System.Net.Sockets.UdpClient udpClient = null;
        private Thread thread = null;
        private volatile bool shouldStop = false;

        public UDPStream(IPConfig ipconf)
            : base(ipconf)
        {
            if (ipconfig.ContainsKey("RECEIVE_PORT"))
            {
                ipconfig["USE_RECEIVER"] = "true"; // 受信する
            }
            if (ipconfig.ContainsKey("SEND_PORT"))
            {
                ipconfig["USE_SENDER"] = "true"; // 送信する
            }


            // 受信設定
            if (ipconfig.ContainsKey("USE_RECEIVER"))
            {
                //バインドするローカルIPとポート番号
                ipconfig["LOCAL_IP"] = "127.0.0.1";
                if (ipconfig.ContainsKey("RECEIVE_IP"))
                {
                    ipconfig["LOCAL_IP"] = ipconfig["RECEIVE_IP"];
                }
                System.Net.IPAddress localIP = System.Net.IPAddress.Parse(ipconfig["LOCAL_IP"]);
                ipconfig["LOCAL_PORT"] = ipconfig["RECEIVE_PORT"];

                //UdpClientを作成し、ローカルエンドポイントにバインドする
                System.Net.IPEndPoint localEP = new System.Net.IPEndPoint(localIP, int.Parse(ipconfig["LOCAL_PORT"]));
                udpClient = new System.Net.Sockets.UdpClient(localEP);
                udpClient.Client.ReceiveTimeout = 5*1000;
            }

            // 送信設定
            if (ipconfig.ContainsKey("USE_SENDER")) // port指定がない場合、送信しない
            {
                ipconfig["REMOTE_IP"] = "127.0.0.1";
                if (ipconfig.ContainsKey("SEND_IP"))
                {
                    ipconfig["REMOTE_IP"] = ipconfig["SEND_IP"];
                }
                ipconfig["REMOTE_PORT"] = ipconfig["SEND_PORT"];

                if (null == udpClient) // UdpClient生成済みなら生成しない。（これいいかは知らん。。。)
                {
                    udpClient = new System.Net.Sockets.UdpClient();
                }
            }
        }

        public override void Open()
        {
            if (!ipconfig.ContainsKey("USE_RECEIVER"))
            {
                return;
            }
            
            thread = new Thread(new ThreadStart(HandleMessage));
            shouldStop = false;
            thread.Start();
            NotifyStatusChanged(this, Stream.Status.Connect);

        }

        public override void Close()
        {
            if (!ipconfig.ContainsKey("USE_RECEIVER"))
            {
                return;
            }
            shouldStop = true;
            thread.Join();
            thread = null;
            NotifyStatusChanged(this, Stream.Status.Disconnect);
        }

        public override void Write(byte[] bytes)
        {
            if (!ipconfig.ContainsKey("USE_SENDER"))
            {
                return;
            }
            udpClient.Send(bytes, bytes.Length, ipconfig["REMOTE_IP"], int.Parse(ipconfig["REMOTE_PORT"]));
        }

        public void HandleMessage()
        {
            while (true)
            {
                if (shouldStop) break;
                try
                {
                    System.Net.IPEndPoint remoteEP = null;
                    byte[] rcvBytes = udpClient.Receive(ref remoteEP);
                    NotifyMessageReceived(this, rcvBytes);
                    // Console.WriteLine("受信したデータ:{0}", rcvBytes);
                }
                catch (System.Net.Sockets.SocketException ex)
                {
                    if (ex.SocketErrorCode == System.Net.Sockets.SocketError.TimedOut)
                    {
                        continue;
                    }
                    // 異常 ERROR
                    throw ex;
                }
                catch (Exception ex)
                {
                    // 異常 ERROR
                    throw ex;
                }

                /*
                //データを文字列に変換する
                string rcvMsg = System.Text.Encoding.UTF8.GetString(rcvBytes);

                //受信したデータと送信者の情報を表示する
                Console.WriteLine("受信したデータ:{0}", rcvMsg);
                Console.WriteLine("送信元アドレス:{0}/ポート番号:{1}",
                    remoteEP.Address, remoteEP.Port);
                */
            }
        }
    }
}
