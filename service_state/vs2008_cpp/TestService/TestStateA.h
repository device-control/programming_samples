#ifndef _TEST_STATE_A_H_
#define _TEST_STATE_A_H_

#include "Common/Singleton.h"
#include "TestState.h"

class TestStateA
	: public TestState
{
public:
	TestStateA();
	~TestStateA();

	virtual void event1(Service& sv, Event& ev);
	virtual void event2(Service& sv, Event& ev);
};

#endif // _TEST_STATE_A_H_

