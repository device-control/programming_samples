#ifndef _THREAD_H_
#define _THREAD_H_

#include "basetype.h"
#include "uncopyable.h"
#include <string>
#include <windows.h>
#include <process.h>

typedef _beginthreadex_proc_type ThreadFunction;
typedef void* ThreadArgs;
typedef HANDLE ThreadHandle;

class Thread: private Uncopyable
{
private:
	/* members */
	std::string m_name; /* thread name */
	ThreadFunction m_function; /* thread name */
	ThreadArgs m_args; /* thread args */
	ThreadHandle m_handle; /* thread handle */
	bool32 m_started; /* thread started */
public:
	Thread(std::string name, ThreadFunction function);
	~Thread();
	void start(ThreadArgs args); /* start of thread */
	void raise(); /* raise of thread */
	void join(); /* join of thread */
};

#endif // _THREAD_H_
