////////////////////////////////////////////////////////////////////////////////

//! @file		TestState.h
//! @brief		ステートクラスの宣言・定義
//! @attention	

////////////////////////////////////////////////////////////////////////////////
#ifndef _TEST_STATE_H_
#define _TEST_STATE_H_

#include "Common/State.h"

////////////////////////////////////////////////////////////////////////////////

//! @brief		ステートクラス
//! @attention	

////////////////////////////////////////////////////////////////////////////////
class API TestState
	: public State
{

private:
protected:
public:
	TestState(std::string name);
	~TestState();

	// イベント実行
	virtual void executeEvent(Service& sv, Event& ev);

	// イベント1
	virtual void event1(Service& sv, Event& ev);
	// イベント2
	virtual void event2(Service& sv, Event& ev);

};

#endif // _TEST_STATE_H_
