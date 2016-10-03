#ifndef _THREAD_LOCKER_H_
#define _THREAD_LOCKER_H_

#include <windows.h>

class ThreadLocker
{
 public:
    ThreadLocker(CRITICAL_SECTION &locker);
    ~ThreadLocker();
 private:
    CRITICAL_SECTION &m_locker;
    
    ThreadLocker(); // ‹ÖŽ~
    ThreadLocker& operator=(const ThreadLocker&);
};

#endif // _THREAD_LOCKER_H_
