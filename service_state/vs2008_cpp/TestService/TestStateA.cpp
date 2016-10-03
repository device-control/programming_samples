#include "Common/Locker.h"
#include "Common/Service.h"
#include "Common/TimerManager.h"
#include "TestService.h"
#include "TestStateA.h"

TestStateA::TestStateA()
    : TestState("TestStateA")
{
}

TestStateA::~TestStateA()
{
}

void TestStateA::event1(Service& sv, Event& ev)
{
	printf("TestStateA::event1()\n");
	TimerManager::getInstance().setTimer(*((TestService*)&sv), "test", 1, 1000*2);
	sv.changeState("TestStateB");
}

void TestStateA::event2(Service& sv, Event& ev)
{
	printf("TestStateA::event2()\n");
}

