#ifndef _LOG_H_
#define _LOG_H_

#include <windows.h>
#include <string>
#include "switchport.h"

class API Log
{
	static const unsigned int LEVEL_SYSTEM;   // 0x00000001 �V�X�e���ŕ\������ƌ��߂�����
	static const unsigned int LEVEL_ERROR;    // 0x00000002 �G���[����
	static const unsigned int LEVEL_WARRNING; // 0x00000004 ���[�j���O����   (�����[�X�r���h���͕K���n�e�e�ɂ���^�p�Ƃ�������)
	static const unsigned int LEVEL_MESSAGE;  // 0x00000008 �����̃��b�Z�[�W (�����[�X�r���h���͕K���n�e�e�ɂ���^�p�Ƃ�������)
	                                          // ���̑��̓��[�U���R
protected:
	CRITICAL_SECTION m_locker;
	unsigned int m_level;
public:
	// �R���X�g���N�^
	Log();
	// �f�X�g���N�^
	virtual ~Log();
	virtual bool open();
	virtual bool write(unsigned int level,  const char* format, ...);
	virtual bool close();
	void setLevel(unsigned int level);
};

#endif	// _LOG_H_
