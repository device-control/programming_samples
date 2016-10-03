using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
//MessageQueueを利用する場合、以下の操作が必要らしい。
//Windows 7 または Windows Vista にメッセージ キュー 4.0 をインストールするには
//コントロール パネルを開きます。
//[プログラム] をクリックし、[プログラムと機能] の [Windows の機能の有効化または無効化] をクリックします。
//[Microsoft Message Queue (MSMQ) Server]、[Microsoft Message Queue (MSMQ) Server Core] の順に展開して、インストールする次のメッセージ キュー機能のチェック ボックスをオンにします。
//[MSMQ Active Directory Domain Services Integration] (ドメインに参加するコンピュータの場合)
//[MSMQ HTTP サポート]
//[OK] をクリックします。
//コンピュータを再起動するかを確認するメッセージが表示されたら、[OK] をクリックしてインストールを完了します。
// https://msdn.microsoft.com/ja-jp/library/aa967729(v=vs.110).aspx
// http://www.atmarkit.co.jp/fdotnet/chushin/introwinform_05/introwinform_05_03.html
//
//上記操作を行ってもアセンブリ参照が不足しているので System.Messagingを追加する必要がある
//メニュー＞プロジェクト＞参照の追加
//から「System.Messaging」を選択。（するだけでいいと思うけど、ゲイツはよーわからん。。。）
//
using System.Messaging;

namespace ServiceState.Common
{
    public abstract class Service : ServiceObservable, IDisposable
    {
        private static readonly object padlock = new object();
        private Dictionary<string, State> states = new Dictionary<string, State>();
        private MessageQueue messageQueue = null;
        private Thread thread = null;
        private volatile bool shouldStop = false;
        private State currentState = null;

        public string Name { get; set; }
        public string StartStateName { get; set; }
        

        protected Service(string name)
        {
            this.Name = name;
            var messageQueueName = @".\Private$\" + name;
            if (MessageQueue.Exists(messageQueueName))
            {
                // 存在していれば削除しておく
                MessageQueue.Delete(messageQueueName);
            }
            messageQueue = MessageQueue.Create(messageQueueName);
        }

        ~Service()
        {
            Dispose(false);
        }

        public void Dispose()
        {
            Dispose(true);
            // Dispose() によってリソースの解放を行ったので、
            // GC での解放が必要が無いことを GC に通知します。
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (disposing)
            {
                // 管理（managed）リソースの破棄処理をここに記述します。 
            }

            // 非管理（unmanaged）リソースの破棄処理をここに記述します。
            Stop();
        }


        // サービスで利用するイベント名を登録
        // service.SetMQEvents(new Type[] { typeof(mqevent.MQEvent1), typeof(mqevent.MQEvent2), typeof(mqevent.MQEvent3) });
        public void SetReceiveMQEvents(Type[] event_types)
        {
            messageQueue.Formatter = new XmlMessageFormatter(event_types); // new Type[] { typeof(MQEvent), typeof(mqevent.MQEventStop) });
        }

        public void Start()
        {
            currentState = FindState(StartStateName);
            thread = new Thread(new ThreadStart(HandleEvent));
            shouldStop = false;
            thread.Start();
        }

        public void Stop()
        {
            if (shouldStop)
            {
                return;
            }
            shouldStop = true;
            thread.Join();
            thread = null;
        }


        public void HandleEvent()
        {
            while (true)
            {
                if (shouldStop) break;
                try
                {
                    Message message = messageQueue.Receive(TimeSpan.FromSeconds(5.0)); // TODO: とりあえず５秒まったら終了チェックするようにした。
                    MQEvent ev = (MQEvent)message.Body;
                    // Display message information.
                    Console.WriteLine("Event name: " + ev.Name);
                    currentState.HandleEvent(this, ev);
                }
                catch (MessageQueueException mqe)
                {
                    if (mqe.MessageQueueErrorCode == MessageQueueErrorCode.IOTimeout) continue;
                    // タイムアウトでなければ、エラーを発生し、エラーログに記録する
                    //LogError("Error : " + mqe.Message);
                    throw mqe;
                }
                catch (Exception e)
                {
                    throw e;
                }
            }
        }

        public void AddState(State state)
        {
            states.Add(state.Name, state);
        }

        public State FindState(string name)
        {
            // if(!states.ContainsValue(name)) "ERROR"
            return states[name];
        }

        public void AddEvent(MQEvent ev)
        {
            messageQueue.Send(ev);
        }

        public void ChangeState(string name)
        {
            currentState = FindState(name);
            NotifyServiceStateChanged(this, name);
        }
    }
}
