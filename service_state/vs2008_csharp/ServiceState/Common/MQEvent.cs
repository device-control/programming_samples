using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ServiceState.Common
{
    // パブリックにしないと保護レベルで送信できなくなる。
    public class MQEvent
    {
        public string Name { get; set; }

        protected MQEvent(string name)
        {
            Name = name;
        }

        // 引数なしのコンストラクタがないとシリアライズできない。
        public MQEvent()
        {
            Name = "";
        }

    }
}
