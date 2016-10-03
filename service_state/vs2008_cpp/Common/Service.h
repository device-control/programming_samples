#ifndef _SERVICE_H_
#define _SERVICE_H_

#ifdef _MSC_VER
    // warning C4251�ւ̑Ή�
    #pragma warning(push)
    #pragma warning(disable:4251)
#endif

#include <windows.h>
#include <process.h>
#include <string>
#include <map>

#include "MSGQueue.h"
#include "Event.h"
#include "switchport.h"

class State;
class API Service
{

private:
	// �X�e�[�^�X�̃}�b�v
	std::map<std::string, State*> m_stateMap;
	// ���݂̃X�e�[�^�X
	State* m_currentState;
	
	// �C�x���g���[�v�p�X���b�h
	HANDLE m_hLoopEventThread;
	// �C�x���g�L���[
	MSGQueue<Event>* m_eventList;

	// �T�[�r�X�J�n�ς݃t���O
	bool m_isOpen;
protected:

public:
	// �R���X�g���N�^
	Service();
	// �f�X�g���N�^
	virtual ~Service();

	// �T�[�r�X�J�n
	void open();
	// �T�[�r�X�I��
	void close();
	// �C�x���g�ǉ�
	void addEvent(Event& ev);
	// ���݂̃X�e�[�g���擾
	State& getCurrentState() const;
	// �X�e�[�g�J��
	void changeState(std::string stateName);
protected:
	// �X�e�[�g�o�^
	void addState(State& state);
	// �X�e�[�g����
	State* findState(std::string stateName) const;

	// �C�x���g���[�v
	void LoopEvent();
	// �C�x���g���[�v��������
	virtual void LoopEventInitialize();
	// �C�x���g���[�v�I������
	virtual void LoopEventFinalize();
	// �C�x���g���s
	void executeEvent(Event& ev);

	// �C�x���g���[�v�X���b�h�̐���
	static unsigned __stdcall eventFunc(void* param);
};

#ifdef _MSC_VER
    #pragma warning(pop)
#endif

#endif	// _SERVICE_H_
