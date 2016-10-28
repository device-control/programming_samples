#include <windows.h>
#include <stdio.h>
#include "Common/ServiceManager.h"
#include "Common/TimerManager.h"
#include "Common/ITimerListener.h"
#include "TestService/TestService.h"
#include "Common/Crypto.h"

class MockTimerLisener: public ITimerListener
{
	virtual void Timeout(std::string name, int sequenceNo)
	{
		printf("%s %d\n", name.c_str(), sequenceNo);
	}
};

int main()
{
	// 暗号化／複合化
	Crypto& cp = *new Crypto("test");
	std::string tbl[] = {
		"123456789",
		"表示能力�@�A�B",
		"表1示2能3力4",
	};
	int tbl_max = sizeof(tbl) / sizeof(tbl[0]);
	for(int i=0;i<tbl_max;i++){
		std::string hirabun = tbl[i];
		std::string enc;
		cp.enc(hirabun, enc);
		std::string dec;
		cp.dec(enc, dec);
		if( hirabun != dec ){
			printf("[%d]異常\n", i);
		}
	}

	MockTimerLisener timerLisener;
	printf("timer start\n");
	TimerManager& tm = TimerManager::getInstance();
	tm.start();
	tm.setTimer(timerLisener, "test", 1, 1000*3);
	for(int i=0;i<5;i++){
		Sleep(1000*1);
		printf("wait %d sec\n", i+1);
	}
	//tm.stop();
	
	ServiceManager& sm = *new ServiceManager(); // ServiceManagerを生成
	sm.addService("TestService", *new TestService()); // ServiceManagerにTestServiceを登録
	TestService& ts = *((TestService *)sm.findService("TestService")); // ServiceManagerに登録したTestServiceを取得
	Event ev("event1", ts.nextSequenceNo()); // イベント生成
	sm.addEvent("TestService", ev); // ServiceManager経由でイベントを登録
	ts.waitEnd();
	delete &sm; // ServiceManagerを削除(serviceが保持しているstate破棄、ServiceManagerが保持しているservice破棄）
}