#include "Common/tcp.h"
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

void Test_Crypto()
{
	printf("--- %s ---\n", __FUNCTION__);
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
}


void Test_Date()
{
	printf("--- %s ---\n", __FUNCTION__);
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
}

void Test_LogManager()
{
	printf("--- %s ---\n", __FUNCTION__);
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

void Test_SoundPlayer()
{
	printf("--- %s ---\n", __FUNCTION__);
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
}

void Test_Service()
{
	printf("--- %s ---\n", __FUNCTION__);
	ServiceManager& sm = *new ServiceManager(); // ServiceManager�𐶐�
	sm.addService("TestService", *new TestService()); // ServiceManager��TestService��o�^
	TestService& ts = *((TestService *)sm.findService("TestService")); // ServiceManager�ɓo�^����TestService���擾
	Event ev("event1", ts.nextSequenceNo()); // �C�x���g����
	sm.addEvent("TestService", ev); // ServiceManager�o�R�ŃC�x���g��o�^
	ts.waitEnd();
	delete &sm; // ServiceManager���폜(service���ێ����Ă���state�j���AServiceManager���ێ����Ă���service�j���j
}


unsigned __stdcall tcp_server_func(void *params)
{
	TCPServer& tcpServer = *new TCPServer();
	tcpServer.open("localhost", 8081);
	// tcpServer.open(std::string("localhost"), 8081);
	TCPSocket& socket = tcpServer.accept();
//	if( NULL == pSocket ){
//		// �ُ�
//		printf("0: SERVER_LOG: ERROR\n");
//	}
//	TCPSocket& socket = *pSocket; // �ʓ|�Ȃ�ŎQ�ƌ^�ɂ���
	// send
	std::string send_msg = "server to hello";
	printf("1: SERVER_LOG: SEND: %s\n", send_msg.c_str());
	std::vector<int8> send_buffer(send_msg.begin(), send_msg.end());
	int32 send_len = socket.send(send_buffer);
	// receive
	std::vector<int8> recv_buffer(20, 0);
	int32 recv_len = socket.receive(recv_buffer, 0);
	std::string recv_msg((const char*)&recv_buffer[0], recv_len);
	printf("4: SERVER_LOG: RECV: %s\n", recv_msg.c_str());
	socket.close();
	tcpServer.close();
	delete &tcpServer;
	Sleep(2 * 1000); // thread �I����҂�����
	printf("5: SERVER_LOG: END\n");
	return 0;
}

void Test_tcp()
{
	printf("--- %s ---\n", __FUNCTION__);
	// tcp
	tcp_initialize();
	{
		/*
		// ����
		struct sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));
		SOCKET m_sock = ::socket(AF_INET, SOCK_STREAM, 0);
		if( INVALID_SOCKET == m_sock ) {
		}
		addr.sin_family = AF_INET;
		addr.sin_port = htons((uint16)8081);
		addr.sin_addr.S_un.S_addr = INADDR_ANY;
		if( 0 != ::bind(m_sock, (struct sockaddr *)&addr, sizeof(addr)) ){
		}
		if( 0 != ::listen(m_sock, 1) ){
		}
		
		TCPSocket& socket = *new TCPSocket();
		socket.open(std::string("127.0.0.1"), 8081);
		*/
	}
	Thread& tcp_server_thread = *new Thread("tcp_server", tcp_server_func);
	tcp_server_thread.start((void*)NULL);
	// HANDLE hThread = reinterpret_cast<HANDLE>(
	// 										  _beginthreadex(
	// 														 NULL,
	// 														 0,
	// 														 tcp_server_func,
	// 														 0,
	// 														 0,
	// 														 0)
	//										  );
	Sleep(1*1000); // thread �����܂ł܂B�K��
	// client connection
	TCPSocket& socket = *new TCPSocket();
	socket.open("127.0.0.1", 8081);
	// receive
	std::vector<int8> recv_buffer(20, 0);
	int32 recv_len = socket.receive(recv_buffer, 0);
	std::string recv_msg((const char*)&recv_buffer[0], recv_len);
	printf("2: CLIENT_LOG: RECV: %s\n", recv_msg.c_str());
	// send
	std::string send_msg = "client to hello";
	printf("3: CLIENT_LOG: SEND: %s\n", send_msg.c_str());
	std::vector<int8> send_buffer(send_msg.begin(), send_msg.end());
	int32 send_len = socket.send(send_buffer);
	socket.close();
	
	//tcpServerThread.join();
	// WaitForSingleObject(hThread, INFINITE);
	// hThread = NULL;
	tcp_server_thread.join();
	printf("6: CLIENT_LOG: END\n");
	delete &tcp_server_thread;

	tcp_finalize();
	printf("7: END\n");
	Sleep(5 * 1000);
}

unsigned __stdcall thread1_func(void *params)
{
	printf("thread start\n");
	int32 i = 0;
	while(1){
		Sleep(1*1000);
		i++;
		printf("thread: %d sec\n",(int)i);
	}
}

void Test_thread1()
{
	printf("--- %s ---\n", __FUNCTION__);
	Thread& thread = *new Thread("thread_test", thread1_func);
	thread.start(NULL);
	Sleep(3*1000);
	printf("thread cancel\n");
	thread.raise();
	thread.join();
	delete &thread;
	printf("thread test end\n");
}

unsigned __stdcall thread2_func(void *params)
{
	Thread& thread = *((Thread*)params);
	// printf("thread test2 start\n");
	int32 i = 0;
	while(1){
		Sleep(1*1000);
		i++;
		// printf("thread test2: %d sec\n",(int)i);
		if( i==3 ){
			thread.raise();
		}
	}
}

void Test_thread2()
{
	printf("--- %s ---\n", __FUNCTION__);
	Thread& thread = *new Thread("thread_test2", thread2_func);
	thread.start((void*)&thread);
	thread.join();
	delete &thread;
	// printf("thread test2 end\n");
}

int main()
{
	Test_Crypto();
	Test_Date();
	Test_LogManager();
	Test_SoundPlayer();
	Test_thread1();
	Test_thread2();
	Test_tcp();
	Test_Service();
	printf("plase push enter.\n");
	while(getchar() != '\n');
}
