#include "Common/Locker.h"
#include "Common/Service.h"
#include "TestService.h"
#include "TestStateB.h"

TestStateB::TestStateB()
    : TestState("TestStateB")
{
}

TestStateB::~TestStateB()
{
}

void TestStateB::event1(Service& sv, Event& ev)
{
	printf("TestStateB::event1()\n");
}

void TestStateB::event2(Service& sv, Event& ev)
{
	printf("TestStateB::event2()\n");
	((TestService*)&sv)->setEndEvent(); // I—¹
}

