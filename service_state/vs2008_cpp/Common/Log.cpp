#include "Log.h"

/*static*/
const unsigned int Log::LEVEL_SYSTEM   = 0x00000001; // システムで表示すると決めたもの
const unsigned int Log::LEVEL_ERROR    = 0x00000002; // エラー発生
const unsigned int Log::LEVEL_WARRNING = 0x00000004; // ワーニング発生   (リリースビルド時は必ずＯＦＦにする運用とかがいい)
const unsigned int Log::LEVEL_MESSAGE  = 0x00000008; // ただのメッセージ (リリースビルド時は必ずＯＦＦにする運用とかがいい)

////////////////////////////////////////////////////////////////////////////////
Log::Log()
	: m_level(0xffffffff) // なんでも表示状態
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
	if( m_pCrypto ) return; // 設定済みの場合
	m_pCrypto = &crypto;
}
