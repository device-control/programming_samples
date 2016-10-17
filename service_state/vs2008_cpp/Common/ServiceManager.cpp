#include "ServiceManager.h"
#include "Service.h"
#include "Event.h"
#include "ThreadLocker.h"

////////////////////////////////////////////////////////////////////////////////
ServiceManager::ServiceManager()
{
	m_serviceMap.clear();
	InitializeCriticalSection(&m_serviceMapLock);
}

////////////////////////////////////////////////////////////////////////////////
ServiceManager::~ServiceManager()
{
	deleteService();
	DeleteCriticalSection(&m_serviceMapLock);
}

////////////////////////////////////////////////////////////////////////////////
void ServiceManager::addService(const std::string& serviceName, Service& service)
{
	ThreadLocker l(m_serviceMapLock);
	m_serviceMap.insert(std::make_pair(serviceName, &service));
}

////////////////////////////////////////////////////////////////////////////////
void ServiceManager::removeService(const std::string& serviceName)
{
	ThreadLocker l(m_serviceMapLock);
	m_serviceMap.erase(serviceName);
}

////////////////////////////////////////////////////////////////////////////////
Service* ServiceManager::findService(const std::string& serviceName)
{
	ThreadLocker l(m_serviceMapLock);
	std::map<std::string, Service*>::const_iterator iter = m_serviceMap.find(serviceName);
	if(iter != m_serviceMap.end()){
		return (*iter).second;
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////
void ServiceManager::deleteService()
{
	std::map<std::string, Service*>::const_iterator iter = m_serviceMap.begin();
	while(iter != m_serviceMap.end()){
		delete (*iter).second;
		++iter;
	}
	m_serviceMap.clear();
}

////////////////////////////////////////////////////////////////////////////////
void ServiceManager::addEvent(const std::string& serviceName, Event& event)
{
	ThreadLocker l(m_serviceMapLock);
	Service* service = findService(serviceName);
	if (NULL != service) {
		service->addEvent(event);
	}
}

////////////////////////////////////////////////////////////////////////////////
void ServiceManager::addEvent(const std::string& serviceName, const std::string& eventName)
{
	ThreadLocker l(m_serviceMapLock);
	Service* service = findService(serviceName);
	if (NULL != service) {
		Event ev(eventName);
		service->addEvent(ev);
	}
}

////////////////////////////////////////////////////////////////////////////////
void ServiceManager::addEvent(const std::string& serviceName, const std::string& eventName, const std::string& entity)
{
	ThreadLocker l(m_serviceMapLock);
	Service* service = findService(serviceName);
	if (NULL != service) {
		Event ev(eventName, entity);
		service->addEvent(ev);
	}
}
