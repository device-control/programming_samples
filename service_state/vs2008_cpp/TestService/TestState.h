////////////////////////////////////////////////////////////////////////////////

//! @file		TestState.h
//! @brief		�X�e�[�g�N���X�̐錾�E��`
//! @attention	

////////////////////////////////////////////////////////////////////////////////
#ifndef _TEST_STATE_H_
#define _TEST_STATE_H_

#include "Common/State.h"

////////////////////////////////////////////////////////////////////////////////

//! @brief		�X�e�[�g�N���X
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

	// �C�x���g���s
	virtual void executeEvent(Service& sv, Event& ev);

	// �C�x���g1
	virtual void event1(Service& sv, Event& ev);
	// �C�x���g2
	virtual void event2(Service& sv, Event& ev);

};

#endif // _TEST_STATE_H_
