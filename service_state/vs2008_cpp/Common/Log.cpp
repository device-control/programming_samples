#include "Log.h"

/*static*/
const unsigned int Log::LEVEL_SYSTEM   = 0x00000001; // �V�X�e���ŕ\������ƌ��߂�����
const unsigned int Log::LEVEL_ERROR    = 0x00000002; // �G���[����
const unsigned int Log::LEVEL_WARRNING = 0x00000004; // ���[�j���O����   (�����[�X�r���h���͕K���n�e�e�ɂ���^�p�Ƃ�������)
const unsigned int Log::LEVEL_MESSAGE  = 0x00000008; // �����̃��b�Z�[�W (�����[�X�r���h���͕K���n�e�e�ɂ���^�p�Ƃ�������)

////////////////////////////////////////////////////////////////////////////////
Log::Log()
	: m_level(0xffffffff) // �Ȃ�ł��\�����
	, m_pCrypto(NULL)
{
	InitializeCriticalSection(&m_locker);
}

////////////////////////////////////////////////////////////////////////////////
Log::~Log()
{
	if(m_pCrypto){
		delete m_pCrypto;
		m_pCrypto = NULL;
	}
	DeleteCriticalSection(&m_locker);
}

////////////////////////////////////////////////////////////////////////////////
bool Log::open()
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////
bool Log::write(unsigned int level,  const char* format, ...)
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////
bool Log::close()
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////
void Log::setLevel(unsigned int level)
{
	m_level = level;
}

void Log::setCrypto(Crypto& crypto)
{
	if( m_pCrypto ) return; // �ݒ�ς݂̏ꍇ
	m_pCrypto = &crypto;
}
