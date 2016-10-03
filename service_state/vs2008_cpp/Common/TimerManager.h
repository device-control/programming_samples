#ifndef _TIMER_MANAGER_H_
#define _TIMER_MANAGER_H_

#include "windows.h"
#include <string>
#include <vector>
#include "Singleton.h"
class ITimerListener;

class TimerManager
	: public Singleton<TimerManager>
{
	friend class Singleton<TimerManager>;
public:
	static const int TIME_UNIT;
private:
	typedef struct {
		ITimerListener* pListener; // �ʒm��
		std::string name; // �^�C�}��
		int sequenceNo; // �^�C�}�V�[�P���X
		int timeout; // �^�C���A�E�g�l
	} TIMER_INFO;

	HANDLE m_thread;
	bool m_isRunning;
	bool m_isStop;
	std::vector<TIMER_INFO *> m_info;
	HANDLE m_mutex;
	HANDLE m_eventStop;
private:
	TimerManager();
	~TimerManager();
	
public:
	bool setTimer(ITimerListener& listener, std::string name, int sequenceNo, int timeout);
	bool deleteTimer(std::string name);
	bool start();
	bool stop();

	static unsigned int __stdcall callTimerProc(void *argument);
	void timerProc();
};
	
#endif // _TIMER_MANAGER_H_

