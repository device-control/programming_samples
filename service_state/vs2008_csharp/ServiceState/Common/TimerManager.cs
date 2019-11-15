using System.Timers;
using System.Collections.Generic;
using System.Threading;

namespace ServiceState.Common
{
    public struct TimerInfo
    {
        public string name;
        public int initial_timeout;
        public int timeout;
        public int loopCount;
        public Service sv;
        public Event ev;

        public TimerInfo(string name, int timeout, int loopCount, Service sv, Event ev)
        {
            this.name = name;
            this.initial_timeout = this.timeout = timeout;
            this.loopCount = loopCount;
            this.sv = sv;
            this.ev = ev;
        }
    }

    class TimerManager
    {
        public static readonly int TIMER_CONTINUOUS = -1;
        private readonly int TIMER_INTERVAL = 200; // 200ms
        private static readonly TimerManager instance = new TimerManager();
        private static Mutex m_mutex = new Mutex();
        private System.Timers.Timer m_timer = new System.Timers.Timer();
        Dictionary<string, TimerInfo> m_timerInfos = new Dictionary<string, TimerInfo>();
        private TimerManager()
        {
            m_timer.Enabled = true;
            m_timer.AutoReset = true;
            m_timer.Interval = TIMER_INTERVAL;
            m_timer.Elapsed += new ElapsedEventHandler(OnTimerEvent);
        }

        private void OnTimerEvent(object source, ElapsedEventArgs e)
        {
            m_mutex.WaitOne();
            if (null != m_timer)
            {
                List<string> names = new List<string>(m_timerInfos.Keys);
                foreach (string name in names)
                {
                    TimerInfo tmi = m_timerInfos[name];
                    tmi.timeout -= TIMER_INTERVAL;
                    m_timerInfos[name] = tmi;
                    if ( tmi.timeout <= 0 )
                    {
                        tmi.timeout = tmi.initial_timeout;
                        tmi.sv.AddEvent(tmi.ev);
                        if (tmi.loopCount == TIMER_CONTINUOUS) // 無限リピート
                        {
                            m_timerInfos[name] = tmi;
                            continue;
                        }
                        tmi.loopCount -= 1;
                        if(tmi.loopCount == 0)
                        {
                            m_timerInfos.Remove(name);
                            continue;
                        }
                        m_timerInfos[name] = tmi;
                    }
                }
            }
            m_mutex.ReleaseMutex();
        }

        public bool Add(string name, int timeout, int loopCount, Service sv, Event ev)
        {
            bool result = true;
            m_mutex.WaitOne();
            if( null != m_timer )
            {
                if (m_timerInfos.ContainsKey(name))
                {
                    result = false; // 登録済み
                }
                else
                {
                    m_timerInfos.Add(name, new TimerInfo(name, timeout, loopCount, sv, ev));
                }
            }
            m_mutex.ReleaseMutex();
            return result;
        }

        public bool Remove(string name)
        {
            bool result = true;
            m_mutex.WaitOne();

            if (null != m_timer)
            {
                if (m_timerInfos.ContainsKey(name))
                {
                    m_timerInfos.Remove(name);
                }
                else
                {
                    result = false;   
                }
            }
            m_mutex.ReleaseMutex();
            return result;
        }

        public void Stop()
        {
            m_mutex.WaitOne();

            m_timer.Stop();
            m_timer = null;
            m_timerInfos.Clear();
            m_mutex.Close();

            m_mutex.ReleaseMutex();

        }

        public static TimerManager Instance
        {
            get
            {
                return instance;
            }
        }
    }
}
