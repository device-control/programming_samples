﻿using System.Timers;
using System.Collections.Generic;
using System.Threading;

// Event に依存しない ITimerObserver で実現したタイマ
namespace ServiceState.Common
{
    public struct TimerInformation
    {
        public string name; // タイマ名
        public int initial_timeout; // タイムアウト初期値値
        public int timeout; // タイムアウト値
        public int sequenceNo; // シーケンス番号(ユーザ指定)
        public int count; // 繰り返しカウント 
        public ITimerObserver observer;

        public TimerInformation(string name, int timeout, int sequenceNo, ITimerObserver observer)
        {
            this.name = name;
            this.initial_timeout = this.timeout = timeout;
            this.sequenceNo = sequenceNo;
            this.observer = observer;
            this.count = 0;
        }
    }
    
    public class TimerManager
    {
        private const int DEFUALT_TIMER_INTERVAL = 200; // 200ms
        private int TIMER_INTERVAL = DEFUALT_TIMER_INTERVAL;
        private static readonly TimerManager instance = new TimerManager();
        private object m_mutex = new object();
        private System.Timers.Timer m_timer = null;
        Dictionary<string, TimerInformation> m_timerInfos = null;
        private TimerManager()
        {
        }
        
        // 注意：起動時1回だけ呼び出す
        public bool Start(int interval_time = DEFUALT_TIMER_INTERVAL)
        {
            if( null != this.m_timer)
            {
                return false; // start中
            }
            this.m_timerInfos = new Dictionary<string, TimerInformation>();
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
                    TimerInformation tmi = this.m_timerInfos[name];
                    tmi.timeout -= this.TIMER_INTERVAL;
                    this.m_timerInfos[name] = tmi;
                    if (tmi.timeout <= 0)
                    {
                        tmi.timeout = tmi.initial_timeout;
                        if (false == tmi.observer.Timeout(tmi)) // コールバック
                        {
                            // 継続
                            tmi.count += 1;
                            this.m_timerInfos[name] = tmi;
                        }
                        else
                        {
                            // 終了
                            this.m_timerInfos.Remove(name);
                            continue;
                        }
                    }
                }
            }
        }

        public bool Add(string name, int timeout, int sequenceNo, ITimerObserver observer)
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
                    this.m_timerInfos.Add(name, new TimerInformation(name, timeout, sequenceNo, observer));
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

        public static TimerManager Instance
        {
            get
            {
                return instance;
            }
        }
    }
}
