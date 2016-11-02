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
		"表示能力①②③",
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
	delete &cp;

	// Dateクラス実験
	//std::string& st = std::string("test");
	Date now = Date::getToday();
	int dayOfWeek = Date::getDayOfWeek(2016,10,30);
	if( dayOfWeek == 1/*火*/ ){
		printf("2016/11/1 == 火曜日 OK\n");
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
		// mktime() なら結果が1970年以降でないと動作しない。。。
		Date d4 = d1 - (356*44); // 1971年
		Date d5 = d1 - (356*45); // 1970年
		Date d6 = d1 - (356*46); // 1969年 mktime() はここで失敗する
	}
	catch(std::out_of_range e){
		printf("%s\n", e.what());
	}
	Date d7 = d2 + (365*100); // 100年後(2038年問題は64bitだと問題ないかも。。。)
	// 2200/12/31 まで経過日数が+1されていくか確認。ついでに曜日も確認してみる
	{
		//int d0 = Date::getDays(2000,1,1) - Date::getDays(1900,1,1);
		// 1582年10月4日まではユリウス暦、次の日(10月15日)からはグレゴリオ暦 とするべきだけど、面倒なんでグレゴリオ暦で計算
		// Dateクラス自体は 1900年以降を保証できれば十分なため。
		int days = Date::getDays(1900,1,1);
		Date dc = Date(1900,1,1); 
		int dayOfWeek = Date::getDayOfWeek(1900,1,1); // 月曜日
		for(int y=1900;y<=2200;y++){
			for(int m=1;m<=12;m++){
				int daysInMonth = Date::getDaysInMonth(y,m);
				for(int d=1;d<=daysInMonth;d++){
					int _days = Date::getDays(y,m,d);
					int _dayOfWeek = Date::getDayOfWeek(y,m,d);
					if( days != _days ){
						printf("days異常\n");
					}
					if( dayOfWeek != _dayOfWeek ){
						printf("dayOfWeek異常\n");
					}
					int _days2 = dc.getDays();
					if( days != _days2){
						printf("days2異常\n");
					}
					int _days3 = Date::toInt(y,m,d);
					if( _days != _days3 ){
						printf("days3異常\n");
					}
					days++;
					dc++;
					dayOfWeek++;
					dayOfWeek%=7;
				}
			}
		}
	}

	// LogManagerクラス実験
	LogManager::createDirectory("log"); // dummy directorys
	LogManager::createDirectory("log\\test00");
	LogManager::createDirectory("log\\test01\\test222");
	system("echo 0 >> log\\20171101.log"); // 1年未来
	system("echo 1 >> log\\20161031.log");
	system("echo 2 >> log\\20161030.log");
	system("echo 3 >> log\\20161029.log");
	system("echo 4 >> log\\20161028.log");
	system("echo 4 >> log\\20161027.log");
	system("echo 4 >> log\\20151101.log"); // １年前
	LogManager& lm = *new LogManager("2016/11/1", "log"); // ログ作成日, ログ出力先
	std::string fileName = lm.getFileName();
	std::vector<std::string> list = lm.getFileList();
	lm.dayRetentionOf(3); // 指定引数「3」とは当日の除いて３日間のログを保持。それより過去は削除
	std::vector<std::string> list2 = lm.getFileList();
	// TODO: logファイルにテスト書き出し

	delete &lm;
	LogManager::deleteDirectory("log"); // logフォルダ丸ごと削除

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