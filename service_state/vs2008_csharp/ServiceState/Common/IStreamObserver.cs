using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ServiceState.Common
{
    public interface IStreamObserver
    {
        void StatusChanged(Stream stream, Stream.Status status);
        void MessageReceived(Stream stream, Byte[] bytes);
    }
}
