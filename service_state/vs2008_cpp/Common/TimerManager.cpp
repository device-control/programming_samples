#include <process.h>
#include "TimerManager.h"
#include "ITimerListener.h"
#include "Locker.h"

const int TimerManager::TIME_UNIT = 100; // ms

////////////////////////////////////////////////////////////////////////////////
TimerManager::TimerManager()
	: m_isRunning(false)
	, m_isStop(false)
	, m_thread(0)
	, m_info()
{
	m_mutex = CreateMutex(NULL, FALSE, NULL);
	m_eventStop = CreateEvent(NULL, TRUE, FALSE, NULL);
}

////////////////////////////////////////////////////////////////////////////////
TimerManager::~TimerManager(void)
{
	stop();
	CloseHandle(m_eventStop);
	CloseHandle(m_mutex);
}

////////////////////////////////////////////////////////////////////////////////
bool TimerManager::setTimer(ITimerListener& listener, std::string name, int sequenceNo, int timeout)
{
	Locker lock(m_mutex);

	TimerManager::TIMER_INFO *ti = NULL;
	std::vector<TimerManager::TIMER_INFO *>::iterator it;
	for (it = m_info.begin(); it != m_info.end(); ++it) {
		TimerManager::TIMER_INFO *ip = *it;
		if( ip->name == name ){
			return false;
		}
	}
	TimerManager::TIMER_INFO *pNewTimerInfo= new TimerManager::TIMER_INFO;
	pNewTimerInfo->pListener = &listener;
	pNewTimerInfo->name = name;
	pNewTimerInfo->sequenceNo = sequenceNo;
	pNewTimerInfo->timeout = timeout;
	m_info.push_back(pNewTimerInfo);
	return true;
}
	
////////////////////////////////////////////////////////////////////////////////
bool TimerManager::deleteTimer(std::string name)
{
	Locker lock(m_mutex);

	std::vector<TimerManager::TIMER_INFO *>::iterator it = m_info.begin();
	while (it != m_info.end()) {
		TimerManager::TIMER_INFO *ip = *it;
		if( ip->name == name ){
			it = m_info.erase(it);
			delete ip;
			return true;
		}
		else{
			++it;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////
bool TimerManager::start()
{
	Locker lock(m_mutex);

	if( m_isRunning ) {
		// スレッド起動中なら何もしない
		return false;
	}
	m_thread = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, callTimerProc, this, 0, NULL));
	if (m_thread <= 0) {
		return false;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////
bool TimerManager::stop()
{
	Locker lock(m_mutex);

	if(m_isStop) {
		return false;
	}
	m_isStop = true;
	// スレッド停止待ち
	ResetEvent(m_eventStop);
	SignalObjectAndWait(m_mutex, m_eventStop, INFINITE, FALSE);
	return true;
}

////////////////////////////////////////////////////////////////////////////////
unsigned int __stdcall TimerManager::callTimerProc(void *argument)
{
	TimerManager* manager = reinterpret_cast<TimerManager*>(argument);
	manager->timerProc();
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
void TimerManager::timerProc()
{
	m_isRunning = true; // スレッド起動中
	while(1){
		if( m_isStop ) {
			break;
		}
		Sleep(TimerManager::TIME_UNIT);
		{
			Locker lock(m_mutex);
			std::vector<TimerManager::TIMER_INFO *>::iterator it = m_info.begin();
			while (it != m_info.end()) {
				TimerManager::TIMER_INFO *ip = *it;
				ip->timeout -= TimerManager::TIME_UNIT;
				if( ip->timeout <= 0 ) {
					ip->pListener->Timeout(ip->name, ip->sequenceNo);
					it = m_info.erase(it);
					delete ip;
				}
				else{
					++it;
				}
			}
		}
	}
	m_isRunning = false; // スレッド停止中
	SetEvent(m_eventStop);
}


