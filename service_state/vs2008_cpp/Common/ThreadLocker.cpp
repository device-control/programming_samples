#include "ThreadLocker.h"

////////////////////////////////////////////////////////////////////////////////
ThreadLocker::ThreadLocker(CRITICAL_SECTION &locker) : m_locker(locker)
{
	EnterCriticalSection(&m_locker);
}

////////////////////////////////////////////////////////////////////////////////
ThreadLocker::~ThreadLocker()
{
	LeaveCriticalSection(&m_locker);
}

