using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ServiceState.Common
{
    // singleton
    class ServiceManager
    {
        private static ServiceManager singleton = new ServiceManager();
        private static readonly object padlock = new object();
        private Dictionary<string, Service> services = new Dictionary<string, Service>();
        
        private ServiceManager()
        {
        }

        ~ServiceManager()
        {
        }

        public void AddService(Service service)
        {
            services.Add(service.Name, service);
        }

        public void RemoveService(string name)
        {
            services.Remove(name);
        }

        public Service FindService(string name)
        {
            // if(!services.ContainsValue(name)) "ERROR"
            return services[name];
        }

        public static ServiceManager GetInstance()
        {
            return singleton;
        }
    }
}
