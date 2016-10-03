using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ServiceState.Common
{
    using IPConfig = Dictionary<string, string>;

    public abstract class Stream : StreamObservable
    {
        protected IPConfig ipconfig;

        public enum Status
        {
            Disconnect, // 接続中
            Connect // 切断中
        }

        public Stream(IPConfig ipconfig)
        {
            this.ipconfig = new IPConfig(ipconfig);
        }

        public string GetConfig(string key)
        {
            if (!ipconfig.ContainsKey(key))
            {
                // ERROR
            }
            return ipconfig[key];
        }

        public abstract void Open();
        public abstract void Write(byte[] bytes);
        public abstract void Close();
    }
}
