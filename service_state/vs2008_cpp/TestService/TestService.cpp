#include "common/Locker.h"
#include "common/TimerManager.h"

#include "TestService.h"
#include "TestState.h"
#include "TestState.h"
#include "TestStateA.h"
#include "TestStateB.h"

TestService::TestService()
	: Service()
	, m_sequenceNo(0)
{
	Service::open();
	initialize();

	m_finished = CreateEvent(NULL, TRUE, FALSE, NULL);
	ResetEvent(m_finished);
}

TestService::~TestService()
{
	Service::close();
	CloseHandle(m_finished);
}

void TestService::initialize()
{
	// 状態登録
	addState( *(new TestStateA()) );
	addState( *(new TestStateB()) );
	// 開始状態設定
	setInitialState("TestStateA");
}

int TestService::nextSequenceNo()
{
	return m_sequenceNo++;
}

void TestService::Timeout(std::string name, int sequenceNo)
{
	printf("TestService::Timeout()\n");
	Event ev("event2", nextSequenceNo());
	addEvent(ev);
}

void TestService::setEndEvent()
{
	SetEvent(m_finished);
}

void TestService::waitEnd()
{
	// 終了イベントが発行されるまで呼び出しスレッドを止める。無理やりなのでマネしないこと。
	WaitForSingleObject(m_finished,INFINITE);
}