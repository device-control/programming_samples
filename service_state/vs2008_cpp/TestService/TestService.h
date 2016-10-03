#ifndef _TEST_SERVICE_H_
#define _TEST_SERVICE_H_

#include "Common/Service.h"
#include "Common/Singleton.h"
#include "Common/ITimerListener.h"

class TestService
	: public Service
	, public ITimerListener
	, public Singleton<TestService>
{
private:
	friend class Singleton<TestService>;
	TestService();
	~TestService();
	void initialize();
	virtual void Timeout(std::string name, int sequenceNo);
protected:
public:
	int nextSequenceNo();
	void setEndEvent();
	void waitEnd();
private:
	int m_sequenceNo;
	HANDLE m_finished;
};

#endif // _TEST_SERVICE_H_

