#ifndef _LOCKER_H_
#define _LOCKER_H_

#include <windows.h>
#include "switchport.h"

class API Locker
{
private:
	// �~���[�e�b�N�X
	HANDLE	&m_mutex;	

public:
	// �R���X�g���N�^
	explicit Locker(HANDLE &mutex);
	// �f�X�g���N�^
	~Locker();

	// ���b�N
	void lock();
	// �A�����b�N
	void unlock();
};

#endif	// _LOCKER_H_
