#ifndef _EVENT_H_
#define _EVENT_H_

#include <windows.h>
#include <string>
#include "switchport.h"

class API Event {

private:
	// イベント名
    std::string m_name;
	// エンティティ
    std::string m_entity;
	// シーケンス番号
	int m_sequenceNo;
public:
	// コンストラクタ
	Event();
	Event(const std::string& name, int sequenceNo=0);
	Event(const std::string& name, const std::string& entity, int sequenceNo=0 );
	// デストラクタ
	virtual ~Event();
	// イベント名
	void setName(const std::string& name);
	const std::string& getName() const;
	void setSequenceNo(int sequenceNo);
	short getSequenceNo() const;
	// エンティティ
    void setEntity(const std::string& entity);
    const std::string& getEntity() const;
private:
	

};

#endif	// _EVENT_H_
