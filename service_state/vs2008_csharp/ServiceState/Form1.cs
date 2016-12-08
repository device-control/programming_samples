using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace ServiceState
{
    public partial class Form1 : Form, Common.IServiceObserver
    {
        private Service.UDPTestService testService = null; // new service.TestService();
        private Service.TCPTestService tcpTestService = null;

        public Form1()
        {
            InitializeComponent();
        }

        // このコンストラクタは、例外をスローするコードを呼び出すときには使用しません。
        // そのため、このようなコードは他の場所に配置する必要があります。
        // その場所は Form1_Load メソッドです。
        // コード エディタの上部にある [Form1.cs [デザイン]] タブをクリックして、Windows フォームに戻ります。
        // フォームを選択し、[プロパティ] ウィンドウの [イベント] (稲妻の印が付いたボタン) をクリックし、[Load] をダブルクリックします。
        // これでイベント ハンドラ メソッドが追加され、コード ビュー内のこのメソッドにカーソルが配置されます。
        // 
        // 「デザイナの表示」を選択しＵＩ画面表示し、表示中の画面でマウス右ボタンメニューから
        // 「プロパティ」を選択。カミナリマークをクリックし「Load」をダブルクリックすると以下のコードが付加される
        private void Form1_Load(object sender, EventArgs e)
        {
            // Test
            Test.BitConverterExTest.Test00();
            Test.StructBaseTest.Test00();

            // UDP テスト
            testService = new Service.UDPTestService();
            Common.ServiceManager.GetInstance().AddService(testService);
            testService.AddObserver(this);
            testService.Start();
            testService.AddEvent(new Common.Event("EventA")); // イベント "EventA" 発行


            // TCP テスト
            tcpTestService = new Service.TCPTestService();
            Common.ServiceManager.GetInstance().AddService(tcpTestService);
            tcpTestService.Start();
            tcpTestService.AddEvent(new Common.Event("EventA")); // イベント "EventA" 発行

        }

        // 「デザイナの表示」を選択しＵＩ画面表示し、表示中の画面でマウス右ボタンメニューから
        // 「プロパティ」を選択。カミナリマークをクリックし「FromClosing」をダブルクリックすると以下のコードが付加される
        private void Form1_FormClosing_1(object sender, FormClosingEventArgs e)
        {
            testService.Stop();
            testService.Dispose();
            testService = null;

            tcpTestService.Stop();
            tcpTestService.Dispose();
            tcpTestService = null;
        }

        void Common.IServiceObserver.StateChanged(Common.Service service, string stateName)
        {
            Console.WriteLine("StateChanged {0} {1}", service.Name, stateName);
        }

        // コントロールを名前で取得
        public static Control _FindControl(Control baseControl, string name)
        {
            // baseControl の全コントロール検索
            foreach (Control control in baseControl.Controls)
            {
                // コントロール内にコントロールが含まれている場合は再帰
                if (control.HasChildren)
                {
                    Control cFindControl = _FindControl(control, name);

                    // コントロールが見つかった場合はそのまま返す
                    if (cFindControl != null)
                    {
                        return cFindControl;
                    }
                }

                // コントロール名がチェック
                if (control.Name == name)
                {
                    return control;
                }
            }
            return null;
        }

        // コントロールを名前で取得
        public Control GetControl(string name)
        {
            Control ctl = _FindControl(this, name);
            return ctl;
        }

        // 値を取得(text)
        public string GetControlValueString(string name, string defaultValue)
        {
            var ctrl = GetControl(name);
            if (null == ctrl)
            {
                return defaultValue;
            }
            return (string)ctrl.Text;
        }
        // 値を取得(uint)
        public uint GetControlValueUINT(string name, uint defaultValue)
        {
            var str = GetControlValueString(name, "");
            if (str == "")
            {
                return defaultValue;
            }
            uint num = defaultValue;
            if (UInt32.TryParse(str, out num))
            {
                return num; // 10進数
            }
            return Convert.ToUInt32(str, 16); // 16進数
        }

        // 値を設定(text)
        public bool SetControlValueString(string name, string value)
        {
            var ctrl = GetControl(name);
            if (null == ctrl)
            {
                return false;
            }
            ctrl.Text = value;
            return true;
        }
        // 値を設定(uint)
        delegate void SetControlValueUINTCallBack(string name, uint value);
        public void SetControlValueUINT(string name, uint value)
        {
            //if (m_isCloseing) return; // FormClosing 中は表示させない

            if (this.InvokeRequired)
            {
                SetControlValueUINTCallBack d = new SetControlValueUINTCallBack(SetControlValueUINT);
                this.Invoke(d, new object[] { name, value });
            }
            else
            {
                string str = String.Format("0x{0:X8}", value);
                SetControlValueString(name, str);
            }
        }
    }
}
