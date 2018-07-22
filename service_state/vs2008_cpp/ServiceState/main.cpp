#include <windows.h>
#include <stdio.h>
#include <process.h>
#include "Common/ServiceManager.h"
#include "Common/TimerManager.h"
#include "Common/ITimerListener.h"
#include "TestService/TestService.h"
#include "Common/Crypto.h"
#include "Common/Date.h"
#include "Common/LogManager.h"
#include "Common/LogFile.h"
#include "Common/SoundPlayer.h"
#include "Common/thread.h"

unsigned char OK_WAV[] = {
	#include "wav/ok.wav.txt"
};

unsigned char NG_WAV[] = {
	#include "wav/ng.wav.txt"
};

class MockTimerLisener: public ITimerListener
{
	virtual void Timeout(std::string name, int sequenceNo)
	{
		printf("%s %d\n", name.c_str(), sequenceNo);
	}
};

unsigned __stdcall threadFunc(void *params)
{
	printf("thread start\n");
	int32 i = 0;
	while(1){
		Sleep(1*1000);
		i++;
		printf("thread: %d sec\n",(int)i);
	}
}

void ThreadTest()
{
	printf("--- thread test ---\n");
	Thread& thread = *new Thread("thread_test", threadFunc);
	thread.start(NULL);
	Sleep(3*1000);
	printf("thread cancel\n");
	thread.raise();
	thread.join();
	delete &thread;
	printf("thread test end\n");
}

unsigned __stdcall threadFunc2(void *params)
{
	Thread& thread = *((Thread*)params);
	printf("thread test2 start\n");
	int32 i = 0;
	while(1){
		Sleep(1*1000);
		i++;
		printf("thread test2: %d sec\n",(int)i);
		if( i==3 ){
			thread.raise();
		}
	}
}

void ThreadTest2()
{
	printf("--- thread test2 ---\n");
	Thread& thread = *new Thread("thread_test2", threadFunc2);
	thread.start((void*)&thread);
	thread.join();
	delete &thread;
	printf("thread test2 end\n");
}

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
	delete &cp;

	// Date�N���X����
	//std::string& st = std::string("test");
	Date now = Date::getToday();
	int dayOfWeek = Date::getDayOfWeek(2016,10,30);
	if( dayOfWeek == 1/*��*/ ){
		printf("2016/11/1 == �Ηj�� OK\n");
	}
	Date d1(2014,1,1);
	Date d2 = d1 - 1; // =2013/12/31
	if( d1 > d2 ){
		printf("2014/1/1 > 2013/12/31 OK");
	}
	if( d2 == Date(2013,12,31) ){
		printf("==2013/12/31 OK\n");
	}
	if( d2 != Date(2013,12,30) ){
		printf("!=2013/12/30 OK\n");
	}
	Date d3 = d2 + 1; // =2014/01/01
	if( d3 == Date(2014,1,1) ){
		printf("2014/01/01 OK\n");
	}
	d2 += 1; // 2014/01/01
	d3 -= 1; // 2013/12/31
	if( d2 == d1 ){
		printf("2014/01/01 OK\n");
	}

	try{
		// mktime() �Ȃ猋�ʂ�1970�N�ȍ~�łȂ��Ɠ��삵�Ȃ��B�B�B
		Date d4 = d1 - (356*44); // 1971�N
		Date d5 = d1 - (356*45); // 1970�N
		Date d6 = d1 - (356*46); // 1969�N mktime() �͂����Ŏ��s����
	}
	catch(std::out_of_range e){
		printf("%s\n", e.what());
	}
	Date d7 = d2 + (365*100); // 100�N��(2038�N����64bit���Ɩ��Ȃ������B�B�B)
	// 2200/12/31 �܂Ōo�ߓ�����+1����Ă������m�F�B���łɗj�����m�F���Ă݂�
	{
		//int d0 = Date::getDays(2000,1,1) - Date::getDays(1900,1,1);
		// 1582�N10��4���܂ł̓����E�X��A���̓�(10��15��)����̓O���S���I�� �Ƃ���ׂ������ǁA�ʓ|�Ȃ�ŃO���S���I��Ōv�Z
		// Date�N���X���̂� 1900�N�ȍ~��ۏ؂ł���Ώ\���Ȃ��߁B
		int days = Date::getDays(1900,1,1);
		Date dc = Date(1900,1,1); 
		int dayOfWeek = Date::getDayOfWeek(1900,1,1); // ���j��
		for(int y=1900;y<=2200;y++){
			for(int m=1;m<=12;m++){
				int daysInMonth = Date::getDaysInMonth(y,m);
				for(int d=1;d<=daysInMonth;d++){
					int _days = Date::getDays(y,m,d);
					int _dayOfWeek = Date::getDayOfWeek(y,m,d);
					if( days != _days ){
						printf("days�ُ�\n");
					}
					if( dayOfWeek != _dayOfWeek ){
						printf("dayOfWeek�ُ�\n");
					}
					int _days2 = dc.getDays();
					if( days != _days2){
						printf("days2�ُ�\n");
					}
					int _days3 = Date::toInt(y,m,d);
					if( _days != _days3 ){
						printf("days3�ُ�\n");
					}
					days++;
					dc++;
					dayOfWeek++;
					dayOfWeek%=7;
				}
			}
		}
	}

	{
		// LogManager�N���X����
		LogManager::createDirectory("log"); // dummy directorys
		LogManager::createDirectory("log\\test00");
		LogManager::createDirectory("log\\test01\\test222");
		system("echo 0 >> log\\20171101.log"); // 1�N����
		system("echo 1 >> log\\20161031.log");
		system("echo 2 >> log\\20161030.log");
		system("echo 3 >> log\\20161029.log");
		system("echo 4 >> log\\20161028.log");
		system("echo 4 >> log\\20161027.log");
		system("echo 4 >> log\\20151101.log"); // �P�N�O
		LogManager& lm = *new LogManager("2016/11/1", "log"); // ���O�쐬��, ���O�o�͐�
		std::string fileName = lm.getFileName();
		std::vector<std::string> list = lm.getFileList();
		lm.dayRetentionOf(3); // �w������u3�v�Ƃ͓����̏����ĂR���Ԃ̃��O��ێ��B������ߋ��͍폜
		std::vector<std::string> list2 = lm.getFileList();
		// TODO: log�t�@�C���Ƀe�X�g�����o��
		Crypto& cp = *new Crypto("test");
		LogFile& log = *new LogFile(fileName.c_str());
		log.setCrypto(cp); // cp �Ǘ��� log ���s���idelete �� Log)
		log.open();
		log.write(Log::LEVEL_SYSTEM, "%d:%s\n", 0x1000, "test");
		log.close();
		delete &log;

		
		delete &lm;
		LogManager::deleteDirectory("log"); // log�t�H���_�ۂ��ƍ폜
	}
	
	// sound player test
	SoundPlayer sound_player;
	sound_player.play(NG_WAV, sizeof(NG_WAV));

	MockTimerLisener timerLisener;
	printf("timer start\n");
	TimerManager& tm = TimerManager::getInstance();
	tm.start();
	tm.setTimer(timerLisener, "test", 1, 1000*3);
	for(int i=0;i<50;i++){
		Sleep(500*1);
		printf("wait %d sec\n", i+1);
		(i & 1) ? sound_player.play(OK_WAV, sizeof(OK_WAV)): sound_player.play(NG_WAV, sizeof(NG_WAV));
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
