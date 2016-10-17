#ifndef _SERVICE_MANAGER_H_
#define _SERVICE_MANAGER_H_

#include <string>
#include <map>
#include <windows.h>
#include "Event.h"
#include "Singleton.h"
#include "switchport.h"

class Service;
class API ServiceManager
{

private:
	std::map<std::string, Service*> m_serviceMap;
	CRITICAL_SECTION m_serviceMapLock;

public:
	ServiceManager();
	~ServiceManager();
	void addService(const std::string& serviceName, Service& service);
	void removeService(const std::string& serviceName);
	Service* findService(const std::string& serviceName);
	void deleteService();
	void addEvent(const std::string& serviceName, Event& ev);
	void addEvent(const std::string& serviceName, const std::string& eventName);
	void addEvent(const std::string& serviceName, const std::string& eventName, const std::string& entity);

};

#endif	// _SERVICE_MANAGER_H_
