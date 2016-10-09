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
        private System.Net.Sockets.TcpClient m_tcpClient = null;
        private System.Net.Sockets.TcpListener m_tcpListener = null;
        private System.Net.Sockets.NetworkStream m_networkStream = null;
        private Thread m_thread = null;
        private volatile bool m_is_running = false;

        public TCPServerStream(IPConfig ipconf)
            : base(ipconf)
        {
            // 待ち受け設定
            //ListenするIPアドレス
            System.Net.IPAddress localIP =
                System.Net.IPAddress.Parse(ipconfig["RECEIVE_IP"]);
            //TcpListenerオブジェクトを作成する
            m_tcpListener =
                new System.Net.Sockets.TcpListener(localIP, int.Parse(ipconfig["RECEIVE_PORT"]));
        }

        public override void Open()
        {
            if (m_is_running) return;
            m_thread = new Thread(new ThreadStart(HandleMessage));
            m_is_running = true;
            m_thread.Start();
        }

        public override void Close()
        {
            if (!m_is_running) return;
            m_networkStream.Close();
            m_tcpListener.Stop();
            m_is_running = false;
            m_thread.Join();
            m_thread = null;
        }

        public override void Write(byte[] bytes)
        {
            try
            {
                //送信
                Console.Write("TCPServer Write {0}\n", bytes.ToString());
                m_networkStream.Write(bytes, 0, bytes.Length);
            }
            catch
            {
                Console.Write("TCPServer Write Error\n");
            }
        }

        public void HandleMessage()
        {
            try
            {
                //Listenを開始する
                m_tcpListener.Start();
                while(m_is_running){
                    //接続要求があったら受け入れる
                    m_tcpClient = m_tcpListener.AcceptTcpClient(); // m_tcpListener.Stop() で例外が発行されるはず。
                    //NetworkStreamを取得
                    m_networkStream = m_tcpClient.GetStream();
                    NotifyStatusChanged(this, Stream.Status.Connect); // 接続完了
                    //読み取り、書き込みのタイムアウトを10秒にする
                    //デフォルトはInfiniteで、タイムアウトしない
                    //(.NET Framework 2.0以上が必要)
                    //m_networkStream.ReadTimeout = 1000 * 1;
                    //m_networkStream.WriteTimeout = 1000 * 1;
                    
                    byte[] receiveBytes = new byte[64*1024];
                    int receiveSize = 0;
                    while(m_is_running)
                    {
                        // 受信待ち
                        receiveSize = m_networkStream.Read(receiveBytes, 0, receiveBytes.Length);
                        //0の場合、タイムアウトかクライアント切断
                        if (receiveSize == 0)
                        {
                            if( !m_tcpClient.Connected ) {
                                // 接続中でないならクライアント切断と判断
                                break;
                            }
                            // 接続中ならタイムアウト
                            continue;
                        }
                        byte[] buff = new byte[receiveSize];
                        Buffer.BlockCopy(receiveBytes, 0, buff, 0, receiveSize);
                        NotifyMessageReceived(this, buff);
                    }
                    m_networkStream.Close();
                    m_networkStream = null;
                    m_tcpClient.Close();
                    m_tcpClient = null;
                }
            }
            catch (System.Net.Sockets.SocketException e)
            {
                Console.WriteLine("TCP client error {0}", e.ErrorCode);
            }
            catch
            {
                Console.WriteLine("TCPサーバーを終了しました");
            }
            if(null != m_networkStream)
            {
                m_networkStream.Close();
                m_networkStream = null;
            }
            if(null != m_tcpClient)
            {
                m_tcpClient.Close();
                m_tcpClient = null;
            }
            NotifyStatusChanged(this, Stream.Status.Disconnect); // 切断完了
        }
    }
}
