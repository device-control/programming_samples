using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ServiceState.Common
{
    public abstract class ServiceObservable
    {
        private List<IServiceObserver> observers = new List<IServiceObserver>();

        public void AddObserver(IServiceObserver observer)
        {
            observers.Add(observer);
        }

        protected void NotifyServiceStateChanged(Service service, string stateName)
        {
            observers.ForEach(observer => observer.StateChanged(service, stateName));
        }
    }
}
