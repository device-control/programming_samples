#ifndef _ITIMER_LISTENER_H_
#define _ITIMER_LISTENER_H_

#include <string>

class ITimerListener
{
	
protected:
	ITimerListener(void);

public:
	virtual ~ITimerListener(void);
	virtual void Timeout(std::string name, int sequenceNo) = 0;
};

#endif // _ITIMER_LISTENER_H_
