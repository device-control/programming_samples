#ifndef _MSGQUEUE_H_
#define _MSGQUEUE_H_

#include "Locker.h"
#include <queue>

template<class T>
class MSGQueue
{
  private:
	std::queue<T>	m_queue;
	HANDLE			m_mutex;
	HANDLE			m_notEmpty;
	bool			m_finished;

  public:
	// �R���X�g���N�^
	MSGQueue::MSGQueue(){
		m_mutex = CreateMutex(NULL, FALSE, NULL);
		m_notEmpty = CreateEvent(NULL, TRUE, FALSE, NULL);
		m_finished = false;
	}

	// �f�X�g���N�^
	MSGQueue::~MSGQueue(){
		CloseHandle(m_mutex);
		CloseHandle(m_notEmpty);
	}

	// �ǉ�
	void push(T& value)
	{
		Locker guard(m_mutex);
		bool notify = m_queue.empty();
		m_queue.push(value);
		if (notify) {
			SetEvent(m_notEmpty);
		}
	}

	// �擾
	// �L���[����Ȃ�Apush�����܂őҋ@����
	bool pop(T& value)
	{
		Locker guard(m_mutex);
		// �L���[����Ȃ�Apush�����܂őҋ@����
		while (m_queue.empty()) {
			// �I���ς݂Ȃ�ҋ@���Ȃ�
			if(!m_finished){
				ResetEvent(m_notEmpty);
				SignalObjectAndWait(m_mutex, m_notEmpty, INFINITE, FALSE);
			}
			// �I���ς݂Ȃ�false�Ƃ���
			if(m_finished) {
				return false;
			}
		}
		
		// �L���[����v�f�����o��
		value = m_queue.front();
		m_queue.pop();
		return true;
	}

	// �L���[���󂩂ǂ���
	bool empty(){
		Locker guard(m_mutex);
		return m_queue.empty();
	}

	// �I��
	void finish(){
		Locker guard(m_mutex);
		m_finished = true;
		SetEvent(m_notEmpty);
	}

};

#endif	// _MSGQUEUE_H_
