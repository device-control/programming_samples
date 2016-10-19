using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ServiceState.Common
{
    public abstract class State
    {
        public string Name { get; set; }
        public State(string name)
        {
            Name = name;
        }

        public abstract void HandleEvent(Service service, Event ev);

        // 各ステート前処理
        public virtual void SetUp()
        {
        }
        // 各ステート後処理
        public virtual void TearDown()
        {
        }
    }
}
