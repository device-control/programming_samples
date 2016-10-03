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
	// ��ԓo�^
	addState( TestStateA::getInstance() );
	addState( TestStateB::getInstance() );
	// �J�n��Ԑݒ�
	changeState("TestStateA");
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
	// �I���C�x���g�����s�����܂ŌĂяo���X���b�h���~�߂�B�������Ȃ̂Ń}�l���Ȃ����ƁB
	WaitForSingleObject(m_finished,INFINITE);
}