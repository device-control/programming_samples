using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace ServiceState.Common
{
    using IPConfig = Dictionary<string, string>;

    class SerialStream : Stream
    {
        /* 
         * TODO: 以下のパラメータが存在するかの確認を行うのはStreamManagerで行い
         *       パラメータが正常な場合、本コンストラクタを呼び出すようにしたい。
         * STREAM_NAME  : *ストリーム名
         * PORT_NAME    :  接続ポート名(ex:COM1)
         * BAUD_RATE    :  ボーレート(ex:9600)
         */
        private System.IO.Ports.SerialPort m_serialPort = null;
        private Thread m_thread = null;
        private volatile bool m_is_running = false;

        public SerialStream(IPConfig ipconf)
            : base(ipconf)
        {

            m_serialPort = new System.IO.Ports.SerialPort();
            m_serialPort.PortName = "COM1";
            m_serialPort.BaudRate = 9600;
            m_serialPort.Parity = System.IO.Ports.Parity.None;
            m_serialPort.DataBits = 8;
            m_serialPort.StopBits = System.IO.Ports.StopBits.One;
            m_serialPort.Handshake = System.IO.Ports.Handshake.None;

            if (ipconfig.ContainsKey("PORT_NAME"))
            {
                m_serialPort.PortName = ipconfig["PORT_NAME"];
            }

            // https://msdn.microsoft.com/ja-jp/library/system.io.ports.parity(v=vs.110).aspx
            // 文字列から定義値に変換してるっぽい。
            // (System.IO.Ports.Parity)Enum.Parse(typeof(System.IO.Ports.Parity), parity, true);

            // closeするとIOExceptionが発行されるのでタイムアウトは一旦設定しない。
            // m_serialPort.ReadTimeout = 500;
            // m_serialPort.WriteTimeout = 500;
        }

        public override void Open()
        {
            if (m_is_running) return;
            m_is_running = true;
            m_serialPort.Open();
            m_thread = new Thread(new ThreadStart(HandleMessage));
            m_thread.Start();
            NotifyStatusChanged(this, Stream.Status.Connect);
        }

        public override void Close()
        {
            if (!m_is_running) return;
            m_is_running = false;
            m_serialPort.Close();
            m_thread.Join();
            m_thread = null;
            NotifyStatusChanged(this, Stream.Status.Disconnect);
        }

        public override void Write(byte[] bytes)
        {
            if (!m_is_running) return;
            m_serialPort.Write(bytes, 0, bytes.Length);
            // m_udpClient.Send(bytes, bytes.Length, ipconfig["REMOTE_IP"], int.Parse(ipconfig["REMOTE_PORT"]));
        }

        public void HandleMessage()
        {
            while (m_is_running)
            {
                try
                {
                    byte[] receiveBytes = new byte[m_serialPort.ReadBufferSize];
                    int receiveSize = m_serialPort.Read(receiveBytes, 0, receiveBytes.Length);
                    byte[] buff = new byte[receiveSize];
                    Buffer.BlockCopy(receiveBytes, 0, buff, 0, receiveSize);
                    NotifyMessageReceived(this, buff);
                    // Console.WriteLine("受信したデータ:{0}", rcvBytes);
                }
                catch (System.IO.IOException/* ex*/)
                {
                    // TODO: close されると来ると思う。。。
                    continue;
                }
                // catch (System.IO.Ports.TimeoutException/* ex*/)
                // {
                //     continue;
                // }
                catch (Exception ex)
                {
                    // 異常 ERROR
                    throw ex;
                }
            }
        }
    }
}
