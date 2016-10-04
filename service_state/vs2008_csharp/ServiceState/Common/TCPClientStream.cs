using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace ServiceState.Common
{
    using IPConfig = Dictionary<string, string>;

    class TCPClientStream : Stream
    {
        /* 
         * TODO: 以下のパラメータが存在するかの確認を行うのはStreamManagerで行い
         *       パラメータが正常な場合、本コンストラクタを呼び出すようにしたい。
         * STREAM_NAME  : *ストリーム名
         * SEND_IP   :  送信ＩＰ(無指定の場合、localhost)
         * SEND_PORT :  送信ポート
         */
        private System.Net.Sockets.TcpClient tcpClient = null;
        private System.Net.Sockets.NetworkStream networkStream = null;
        private Thread thread = null;
        private volatile bool shouldStop = false;
        private volatile bool isOpen = false;

        public TCPClientStream(IPConfig ipconf)
            : base(ipconf)
        {
            byte[] buffer = new byte[64*1024];
        }

        public override void Open()
        {
            if(isOpen) return;
            thread = new Thread(new ThreadStart(HandleMessage));
            shouldStop = false;
            thread.Start(); // TODO: 接続完了まで待つ必要がある
            NotifyStatusChanged(this, Stream.Status.Connect);
        }

        public override void Close()
        {
            if(!isOpen) return;
            //サーバとの接続を終了
            shouldStop = true;
            thread.Join();
            thread = null;
            NotifyStatusChanged(this, Stream.Status.Disconnect);
        }

        public override void Write(byte[] bytes)
        {
            //送信
            try
            {
                networkStream.Write(bytes, 0, bytes.Length);
            }
            catch
            {
                Console.Write("TCPClient Write Error\n");
            }

        }

        public void HandleMessage()
        {
            isOpen = true;
            try
            {
                //IPアドレスとポート番号を渡してサーバ側へ接続
                tcpClient = new System.Net.Sockets.TcpClient(ipconfig["SEND_IP"], int.Parse(ipconfig["SEND_PORT"]));
                networkStream = tcpClient.GetStream();
                //読み取り、書き込みのタイムアウトを10秒にする
                //デフォルトはInfiniteで、タイムアウトしない
                //(.NET Framework 2.0以上が必要)
                networkStream.ReadTimeout = 1000 * 10;
                networkStream.WriteTimeout = 1000 * 10;

                int receiveSize = 0;
                byte[] receiveBytes = new byte[64 * 1024];
                while(!shouldStop/*tcpClinet.Connected*/){
                    // 受信待ち
                    receiveSize = networkStream.Read(receiveBytes, 0, receiveBytes.Length);
                    //0の場合、タイムアウトかクライアント切断
                    if (receiveSize == 0)
                    {
                        if( !tcpClient.Connected ) {
                            // 接続中でないならクライアント切断と判断
                            shouldStop = false;
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
            catch
            {
                Console.WriteLine("TCPクライアントを終了しました\n");
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
