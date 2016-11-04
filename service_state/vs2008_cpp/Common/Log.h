#ifndef _LOG_H_
#define _LOG_H_

#include <windows.h>
#include <string>
#include "switchport.h"

class API Log
{
	static const unsigned int LEVEL_SYSTEM;   // 0x00000001 システムで表示すると決めたもの
	static const unsigned int LEVEL_ERROR;    // 0x00000002 エラー発生
	static const unsigned int LEVEL_WARRNING; // 0x00000004 ワーニング発生   (リリースビルド時は必ずＯＦＦにする運用とかがいい)
	static const unsigned int LEVEL_MESSAGE;  // 0x00000008 ただのメッセージ (リリースビルド時は必ずＯＦＦにする運用とかがいい)
	                                          // その他はユーザ自由
protected:
	CRITICAL_SECTION m_locker;
	unsigned int m_level;
public:
	// コンストラクタ
	Log();
	// デストラクタ
	virtual ~Log();
	virtual bool open();
	virtual bool write(unsigned int level,  const char* format, ...);
	virtual bool close();
	void setLevel(unsigned int level);
};

#endif	// _LOG_H_
