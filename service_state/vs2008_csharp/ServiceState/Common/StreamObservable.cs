using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ServiceState.Common
{
    public abstract class StreamObservable
    {
        private List<IStreamObserver> observers = new List<IStreamObserver>();

        public void AddObserver(IStreamObserver observer)
        {
            observers.Add(observer);
        }

        public void NotifyStatusChanged(Stream stream, Stream.Status status)
        {
            observers.ForEach(observer => observer.StatusChanged(stream, status));
        }

        public void NotifyMessageReceived(Stream stream, Byte[] bytes)
        {
            observers.ForEach(observer => observer.MessageReceived(stream, bytes));
        }
    }
}
