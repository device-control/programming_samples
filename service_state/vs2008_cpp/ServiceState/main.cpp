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
	// ˆÃ†‰»^•¡‡‰»
	Crypto& cp = *new Crypto("test");
	std::string tbl[] = {
		"123456789",
		"•\¦”\—Í‡@‡A‡B",
		"•\1¦2”\3—Í4",
	};
	int tbl_max = sizeof(tbl) / sizeof(tbl[0]);
	for(int i=0;i<tbl_max;i++){
		std::string hirabun = tbl[i];
		std::string enc;
		cp.enc(hirabun, enc);
		std::string dec;
		cp.dec(enc, dec);
		if( hirabun != dec ){
			printf("[%d]ˆÙí\n", i);
		}
	}
	delete &cp;

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
	
	ServiceManager& sm = *new ServiceManager(); // ServiceManager‚ğ¶¬
	sm.addService("TestService", *new TestService()); // ServiceManager‚ÉTestService‚ğ“o˜^
	TestService& ts = *((TestService *)sm.findService("TestService")); // ServiceManager‚É“o˜^‚µ‚½TestService‚ğæ“¾
	Event ev("event1", ts.nextSequenceNo()); // ƒCƒxƒ“ƒg¶¬
	sm.addEvent("TestService", ev); // ServiceManagerŒo—R‚ÅƒCƒxƒ“ƒg‚ğ“o˜^
	ts.waitEnd();
	delete &sm; // ServiceManager‚ğíœ(service‚ª•Û‚µ‚Ä‚¢‚éstate”jŠüAServiceManager‚ª•Û‚µ‚Ä‚¢‚éservice”jŠüj
}