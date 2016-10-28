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
	// �Í����^������
	Crypto& cp = *new Crypto("test");
	std::string tbl[] = {
		"123456789",
		"�\���\�͇@�A�B",
		"�\1��2�\3��4",
	};
	int tbl_max = sizeof(tbl) / sizeof(tbl[0]);
	for(int i=0;i<tbl_max;i++){
		std::string hirabun = tbl[i];
		std::string enc;
		cp.enc(hirabun, enc);
		std::string dec;
		cp.dec(enc, dec);
		if( hirabun != dec ){
			printf("[%d]�ُ�\n", i);
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
	
	ServiceManager& sm = *new ServiceManager(); // ServiceManager�𐶐�
	sm.addService("TestService", *new TestService()); // ServiceManager��TestService��o�^
	TestService& ts = *((TestService *)sm.findService("TestService")); // ServiceManager�ɓo�^����TestService���擾
	Event ev("event1", ts.nextSequenceNo()); // �C�x���g����
	sm.addEvent("TestService", ev); // ServiceManager�o�R�ŃC�x���g��o�^
	ts.waitEnd();
	delete &sm; // ServiceManager���폜(service���ێ����Ă���state�j���AServiceManager���ێ����Ă���service�j���j
}