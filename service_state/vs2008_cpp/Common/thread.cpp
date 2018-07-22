#include "basetype.h"
#include "thread.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
/*
  Thread constractor
*/
Thread::Thread(std::string name, ThreadFunction function)
	: m_name(name)
	, m_function(function)
	, m_started(FALSE32)
	, m_args(NULL)
{
	memset(&m_handle,0,sizeof(m_handle));
}

/*
  Thread destractor
*/
Thread::~Thread()
{
	if( m_handle ){
		CloseHandle(m_handle);
	}
}

/*
  Thread start
*/
void Thread::start(ThreadArgs args)
{
	m_args = args;
	m_handle = reinterpret_cast<HANDLE>(
										_beginthreadex(
													   NULL,
													   0,
													   m_function,
													   m_args,
													   0,
													   0
													   )
										);
	if( NULL == m_handle ){
		
	}
	m_started = TRUE32;
}

// memo: http://d.hatena.ne.jp/gikogeek/20070703#20070703f4
void Thread::raise()
{
	if( !m_started ) return;

	ThreadHandle handle = GetCurrentThread();
	if( handle == m_handle ){
		/* Ž©ƒXƒŒƒbƒh */
		_endthreadex(0); // arg = return value
		return;
	}
	bool32 result = (bool32)TerminateThread(m_handle,0);
	if( 0 == result ){
		// error.
	}
}

void Thread::join()
{
	if( !m_started ) return;
	WaitForSingleObject(m_handle, INFINITE);
	m_started = FALSE32;
}

