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
        private System.Net.Sockets.TcpClient m_tcpClient = null;
        private System.Net.Sockets.NetworkStream m_networkStream = null;
        private Thread m_thread = null;
        private volatile bool m_is_running = false;

        public TCPClientStream(IPConfig ipconf)
            : base(ipconf)
        {
            byte[] buffer = new byte[64*1024];
        }

        public override void Open()
        {
            if(m_is_running) return;
            m_thread = new Thread(new ThreadStart(HandleMessage));
            m_is_running = true;
            m_thread.Start();
        }

        public override void Close()
        {
            if(!m_is_running) return;
            //サーバとの接続を終了
            m_networkStream.Close();
            m_is_running = false;
            m_thread.Join();
            m_thread = null;
        }

        public override void Write(byte[] bytes)
        {
            //送信
            try
            {
                Console.Write("TCPClient Write {0}\n", bytes.ToString() );
                m_networkStream.Write(bytes, 0, bytes.Length);
            }
            catch
            {
                Console.Write("TCPClient Write Error\n");
            }

        }

        public void HandleMessage()
        {
            while(m_is_running)
            {
                try
                {
                    m_tcpClient = null;
                    m_networkStream = null;
                    //IPアドレスとポート番号を渡してサーバ側へ接続
                    m_tcpClient = new System.Net.Sockets.TcpClient(ipconfig["SEND_IP"], int.Parse(ipconfig["SEND_PORT"]));
                    m_networkStream = m_tcpClient.GetStream();
                    NotifyStatusChanged(this, Stream.Status.Connect); // 接続完了
                    //読み取り、書き込みのタイムアウトを10秒にする
                    //デフォルトはInfiniteで、タイムアウトしない
                    //(.NET Framework 2.0以上が必要)
                    //m_networkStream.ReadTimeout = 1000 * 1;
                    //m_networkStream.WriteTimeout = 1000 * 1;
                    int receiveSize = 0;
                    byte[] receiveBytes = new byte[64 * 1024];
                    while (m_is_running)
                    {
                        receiveSize = m_networkStream.Read(receiveBytes, 0, receiveBytes.Length);
                        //0の場合、タイムアウトかクライアント切断
                        if (receiveSize == 0)
                        {
                            if (!m_tcpClient.Connected)
                            {
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
                    m_is_running = false;
                }
                catch (System.Net.Sockets.SocketException e)
                {
                    if( e.SocketErrorCode == System.Net.Sockets.SocketError.ConnectionRefused){
                        Console.WriteLine("TCP client timeout");
                        System.Threading.Thread.Sleep(1000 * 1);
                        continue;
                    }
                    m_is_running = false;
                }
                catch
                {
                    Console.WriteLine("TCPクライアントを終了しました\n");
                    m_is_running = false;
                }
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
            NotifyStatusChanged(this, Stream.Status.Disconnect); // 切断
        }
    }
}
