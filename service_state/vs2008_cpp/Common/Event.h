#ifndef _EVENT_H_
#define _EVENT_H_

#include <windows.h>
#include <string>
#include "switchport.h"

class API Event {

private:
	// �C�x���g��
    std::string m_name;
	// �G���e�B�e�B
    std::string m_entity;
	// �V�[�P���X�ԍ�
	int m_sequenceNo;
public:
	// �R���X�g���N�^
	Event();
	Event(const std::string& name, int sequenceNo=0);
	Event(const std::string& name, const std::string& entity, int sequenceNo=0 );
	// �f�X�g���N�^
	virtual ~Event();
	// �C�x���g��
	void setName(const std::string& name);
	const std::string& getName() const;
	void setSequenceNo(int sequenceNo);
	short getSequenceNo() const;
	// �G���e�B�e�B
    void setEntity(const std::string& entity);
    const std::string& getEntity() const;
private:
	

};

#endif	// _EVENT_H_
