#include "common/Locker.h"
#include "common/Service.h"
#include "TestService.h"
#include "TestState.h"

TestState::TestState(std::string name)
    : State(name)
{
}

////////////////////////////////////////////////////////////////////////////////

//! @brief       デストラクタ
//! @attention   

///////////////////////////////////////////////////////////////////////////////
TestState::~TestState()
{
}

void TestState::executeEvent(Service& sv, Event& ev)
{
	printf("TestState::executeEvent()\n");
	if(ev.getName() == "event1"){
		event1(sv,ev);
	}
	else if(ev.getName() == "event2"){
		event2(sv,ev);
	}
	else{
		// 異常
	}
#if 0
	// event1
	if( ev.get() == CS_HEALTH_CHECK_EVENT) {
		event1(sv, ev);
	}
	// event2
	else if( ev.getSeqNo() == CS_HEALTH_CHECK_RES_EVENT) {
		event2(sv, ev);
	}
	else{
		// 異常
	}
#endif
}

void TestState::event1(Service& sv, Event& ev)
{
	// 何もしない
}

void TestState::event2(Service& sv, Event& ev)
{
	// 何もしない
}

