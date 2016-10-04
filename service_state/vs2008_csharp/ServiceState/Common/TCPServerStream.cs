using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace ServiceState.Common
{
    using IPConfig = Dictionary<string, string>;

    class TCPServerStream : Stream
    {
        /* 
         * TODO: 以下のパラメータが存在するかの確認を行うのはStreamManagerで行い
         *       パラメータが正常な場合、本コンストラクタを呼び出すようにしたい。
         * STREAM_NAME  : *ストリーム名
         * RECEIVE_IP   :  待ち受けＩＰ(無指定の場合、localhost)
         * RECEIVE_PORT :  待ち受けポート
         */
        private System.Net.Sockets.TcpClient tcpClient = null;
        private System.Net.Sockets.TcpListener tcpListener = null;
        private System.Net.Sockets.NetworkStream networkStream = null;
        private Thread thread = null;
        private volatile bool shouldStop = false;
        private volatile bool isOpen = false;
        private volatile bool connected = false;

        public TCPServerStream(IPConfig ipconf)
            : base(ipconf)
        {
            // 待ち受け設定
            //ListenするIPアドレス
            System.Net.IPAddress localIP =
                System.Net.IPAddress.Parse(ipconfig["RECEIVE_IP"]);
            //TcpListenerオブジェクトを作成する
            tcpListener =
                new System.Net.Sockets.TcpListener(localIP, int.Parse(ipconfig["RECEIVE_PORT"]));
            //Listenを開始する
            tcpListener.Start();
        }

        public override void Open()
        {
            if(isOpen) return;
            thread = new Thread(new ThreadStart(HandleMessage));
            shouldStop = false;
            thread.Start();
            NotifyStatusChanged(this, Stream.Status.Connect);
        }

        public override void Close()
        {
            if (!isOpen) return;
            shouldStop = true;
            thread.Join();
            thread = null;
            NotifyStatusChanged(this, Stream.Status.Disconnect);
        }

        public override void Write(byte[] bytes)
        {
            try
            {
                //送信
                networkStream.Write(bytes, 0, bytes.Length);
            }
            catch
            {
                Console.Write("TCPServer Write Error\n");
            }
        }

        public void HandleMessage()
        {
            isOpen = true;
            try
            {
                while(!shouldStop){
                    //接続要求があったら受け入れる
                    tcpClient = tcpListener.AcceptTcpClient();
                    //NetworkStreamを取得
                    networkStream = tcpClient.GetStream();
                    //読み取り、書き込みのタイムアウトを10秒にする
                    //デフォルトはInfiniteで、タイムアウトしない
                    //(.NET Framework 2.0以上が必要)
                    networkStream.ReadTimeout = 1000 * 10;
                    networkStream.WriteTimeout = 1000 * 10;
                    
                    byte[] receiveBytes = new byte[64*1024];
                    int receiveSize = 0;
                    connected = true;
                    while(connected && !shouldStop)
                    {
                        // 受信待ち
                        receiveSize = networkStream.Read(receiveBytes, 0, receiveBytes.Length);
                        //0の場合、タイムアウトかクライアント切断
                        if (receiveSize == 0)
                        {
                            if( !tcpClient.Connected ) {
                                // 接続中でないならクライアント切断と判断
                                connected = false;
                            }
                            // 接続中ならタイムアウト
                            continue;
                        }
                        byte[] buff = new byte[receiveSize];
                        Buffer.BlockCopy(receiveBytes, 0, buff, 0, receiveSize);
                        NotifyMessageReceived(this, buff);
                    }
                    networkStream.Close();
                    networkStream = null;
                    tcpClient.Close();
                    tcpClient = null;
                }
            }
            catch
            {
                Console.WriteLine("TCPサーバーを終了しました");
            }
            if(null != networkStream)
            {
                networkStream.Close();
                networkStream = null;
            }
            if(null != tcpClient)
            {
                tcpClient.Close();
                tcpClient = null;
            }
            isOpen = false;
        }
    }
}
