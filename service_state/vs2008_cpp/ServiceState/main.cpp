#include <windows.h>
#include <stdio.h>
#include "Common/ServiceManager.h"
#include "Common/TimerManager.h"
#include "Common/ITimerListener.h"
#include "TestService/TestService.h"

class MockTimerLisener: public ITimerListener
{
	virtual void Timeout(std::string name, int sequenceNo)
	{
		printf("%s %d\n", name.c_str(), sequenceNo);
	}
};

int main()
{
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
	
	ServiceManager& sm = *new ServiceManager(); // ServiceManager‚ð¶¬
	sm.addService("TestService", *new TestService()); // ServiceManager‚ÉTestService‚ð“o˜^
	TestService& ts = *((TestService *)sm.findService("TestService")); // ServiceManager‚É“o˜^‚µ‚½TestService‚ðŽæ“¾
	Event ev("event1", ts.nextSequenceNo()); // ƒCƒxƒ“ƒg¶¬
	sm.addEvent("TestService", ev); // ServiceManagerŒo—R‚ÅƒCƒxƒ“ƒg‚ð“o˜^
	ts.waitEnd();
	delete &sm; // ServiceManager‚ðíœ(service‚ª•ÛŽ‚µ‚Ä‚¢‚éstate”jŠüAServiceManager‚ª•ÛŽ‚µ‚Ä‚¢‚éservice”jŠüj
}