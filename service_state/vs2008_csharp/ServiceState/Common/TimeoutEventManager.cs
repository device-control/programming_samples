using System.Timers;
using System.Collections.Generic;
using System.Threading;

// Event に依存したタイマ
namespace ServiceState.Common
{
    public struct TimeoutEventInfo
    {
        public string name;
        public int initial_timeout;
        public int timeout;
        public int loopCount;
        public Service sv;
        public Event ev;

        public TimeoutEventInfo(string name, int timeout, int loopCount, Service sv, Event ev)
        {
            this.name = name;
            this.initial_timeout = this.timeout = timeout;
            this.loopCount = loopCount;
            this.sv = sv;
            this.ev = ev;
        }
    }
    
    public class TimeoutEventManager
    {
        public static readonly int TIMER_CONTINUOUS = -1;
        private int TIMER_INTERVAL = 200; // 200ms
        private static readonly TimeoutEventManager instance = new TimeoutEventManager();
        private object m_mutex = new object();
        private System.Timers.Timer m_timer = null;
        Dictionary<string, TimeoutEventInfo> m_timerInfos = null;
        private TimeoutEventManager()
        {
        }
        
        // 注意：起動時1回だけ呼び出す
        public bool Start(int interval_time = 200)
        {
            if( null != this.m_timer)
            {
                return false; // start中
            }
            this.m_timerInfos = new Dictionary<string, TimeoutEventInfo>();
            this.TIMER_INTERVAL = interval_time;
            this.m_timer = new System.Timers.Timer();
            this.m_timer.Enabled = true;
            this.m_timer.AutoReset = true;
            this.m_timer.Interval = this.TIMER_INTERVAL;
            this.m_timer.Elapsed += new ElapsedEventHandler(OnTimerEvent);
            return true;
        }

        // 注意：終了時１回だけ呼び出す
        public bool Stop()
        {
            if (null == this.m_timer)
            {
                return false; //  Stop中
            }
            lock (this.m_mutex)
            {
                m_timer.Stop();
                m_timer = null;
                m_timerInfos.Clear();
                m_timerInfos = null;
            }
            this.m_mutex = null; // 無理くり
            return true;
        }

        private void OnTimerEvent(object source, ElapsedEventArgs e)
        {
            lock (this.m_mutex)
            {
                if( null == this.m_timer)
                {
                    return;
                }
                // FIXME: ここで m_timer が null かを確認する必要がある？？
                List<string> names = new List<string>(this.m_timerInfos.Keys);
                foreach (string name in names)
                {
                    TimeoutEventInfo tmi = this.m_timerInfos[name];
                    tmi.timeout -= this.TIMER_INTERVAL;
                    this.m_timerInfos[name] = tmi;
                    if (tmi.timeout <= 0)
                    {
                        tmi.timeout = tmi.initial_timeout;
                        tmi.sv.AddEvent(tmi.ev);
                        if (tmi.loopCount == TIMER_CONTINUOUS) // 無限リピート
                        {
                            this.m_timerInfos[name] = tmi;
                            continue;
                        }
                        tmi.loopCount -= 1;
                        if (tmi.loopCount == 0)
                        {
                            this.m_timerInfos.Remove(name);
                            continue;
                        }
                        this.m_timerInfos[name] = tmi;
                    }
                }
            }
        }

        public bool Add(string name, int timeout, int loopCount, Service sv, Event ev)
        {
            lock (this.m_mutex)
            {
                if( null == this.m_timer)
                {
                    return false;
                }
                if (this.m_timerInfos.ContainsKey(name))
                {
                    return false; // 登録済み
                }
                else
                {
                    this.m_timerInfos.Add(name, new TimeoutEventInfo(name, timeout, loopCount, sv, ev));
                }
            }
            return true;
        }

        public bool Remove(string name)
        {
            lock (this.m_mutex)
            {
                if (null == this.m_timer)
                {
                    return false;
                }
                if (this.m_timerInfos.ContainsKey(name))
                {
                    this.m_timerInfos.Remove(name);
                }
                else
                {
                    return false;
                }
            }
            return true;
        }

        public static TimeoutEventManager Instance
        {
            get
            {
                return instance;
            }
        }
    }
}
