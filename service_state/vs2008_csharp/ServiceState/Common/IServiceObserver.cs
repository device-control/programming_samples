using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ServiceState.Common
{
    public interface IServiceObserver
    {
        void StateChanged(Service service, string stateName);
    }
}
