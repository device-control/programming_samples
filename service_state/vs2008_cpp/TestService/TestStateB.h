#ifndef _TEST_STATE_B_H_
#define _TEST_STATE_B_H_

#include "Common/Singleton.h"
#include "TestState.h"

class TestStateB
	: public TestState
	, public Singleton<TestStateB>
{
private:
	friend class Singleton<TestStateB>;
	TestStateB();
	~TestStateB();

public:
	virtual void event1(Service& sv, Event& ev);
	virtual void event2(Service& sv, Event& ev);
};

#endif // _TEST_STATE_B_H_
