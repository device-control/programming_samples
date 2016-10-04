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
        private Service.TestService testService = null; // new service.TestService();
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
            // UDP テスト
            testService = new Service.TestService();
            Common.ServiceManager.GetInstance().AddService(testService);
            testService.AddObserver(this);
            ///testService.Start();
            testService.AddEvent(new MQEvent.GeneralEvent("EventA")); // イベント "EventA" 発行


            // TCP テスト
            tcpTestService = new Service.TCPTestService();
            Common.ServiceManager.GetInstance().AddService(tcpTestService);
            tcpTestService.Start();
            tcpTestService.AddEvent(new MQEvent.GeneralEvent("EventA")); // イベント "EventA" 発行
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
            if (stateName == "TestStateB")
            {
                testService.AddEvent(new MQEvent.GeneralEvent("EventB"));
            }
            if (stateName == "TestStateC")
            {
                testService.AddEvent(new MQEvent.GeneralEvent("EventC"));
            }
        }
    }
}
