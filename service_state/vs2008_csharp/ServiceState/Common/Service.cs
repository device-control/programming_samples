using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace ServiceState.Common
{
    public abstract class Service : ServiceObservable, IDisposable
    {
        // private static readonly object m_padlock = new object();
        private Dictionary<string, State> m_states = new Dictionary<string, State>();
        
        private Queue<Event> m_events = new Queue<Event>();
        private ManualResetEvent m_my_event = new ManualResetEvent(false);
        
        private Thread m_thread = null;
        private volatile bool m_is_running = false;
        private State m_currentState = null;
        
        public string Name { get; set; }
        public string StartStateName { get; set; }
        
        protected Service(string name)
        {
            this.Name = name;
        }

        ~Service()
        {
            Dispose(false);
        }

        public void Dispose()
        {
            Dispose(true);
            // Dispose() によってリソースの解放を行ったので、
            // GC での解放が必要が無いことを GC に通知
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

        public virtual bool Start()
        {
            if (m_is_running) return false;
            m_is_running = true;
            m_currentState = FindState(StartStateName);
            m_thread = new Thread(new ThreadStart(HandleEvent));
            m_my_event.Reset();
            m_thread.Start();
            return true;
        }

        public virtual bool Stop()
        {
            if (!m_is_running) return false;
            m_is_running = false;
            m_my_event.Set();
            m_thread.Join();
            m_thread = null;
            return true;
        }


        public void HandleEvent()
        {
            try
            {
                while (m_my_event.WaitOne())
                {
                    if (!m_is_running) {
                        break;
                    }
                    
                    Event  ev = null;
                    lock (m_events)
                    {
                        if (m_events.Count > 0)
                        {
                            ev = m_events.Dequeue();
                        }
                        else
                        {
                            m_my_event.Reset();
                        }
                    }
                    if (ev != null)
                    {
                        Console.WriteLine("Service[" + Name + "] State[" + m_currentState.Name + "] Event[" + ev.Name + "]");
                        m_currentState.HandleEvent(this, ev);
                    }
                }
            }
            catch (Exception e)
            {
                throw e;
            }
        }

        public void AddState(State state)
        {
            m_states.Add(state.Name, state);
        }

        public State FindState(string name)
        {
            return m_states[name];
        }

        public void AddEvent(Event event_)
        {
            lock (m_events)
            {
                m_events.Enqueue(event_);
                m_my_event.Set();
            }
        }

        public virtual void ChangeState(string name)
        {
            m_currentState = FindState(name);
            NotifyServiceStateChanged(this, name);
        }

        public State GetCurrentState()
        {
            return m_currentState;
        }
    }
}
