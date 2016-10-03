#ifndef _STATE_H_
#define _STATE_H_

#include <windows.h>
#include <string>

#include "Event.h"
#include "switchport.h"

class Service;
class API State
{

protected:
	// �X�e�[�g��
    std::string m_name;

public:
	// �R���X�g���N�^
	State(std::string name);
	// �f�X�g���N�^
	virtual ~State(){}
	// �X�e�[�g�����擾
    std::string getName() const;

	// �C�x���g���s
	virtual void executeEvent(Service& sv, Event& ev) = 0;

};

#endif	// _STATE_H_
