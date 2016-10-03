#ifndef _LOCKER_H_
#define _LOCKER_H_

#include <windows.h>
#include "switchport.h"

class API Locker
{
private:
	// ミューテックス
	HANDLE	&m_mutex;	

public:
	// コンストラクタ
	explicit Locker(HANDLE &mutex);
	// デストラクタ
	~Locker();

	// ロック
	void lock();
	// アンロック
	void unlock();
};

#endif	// _LOCKER_H_
