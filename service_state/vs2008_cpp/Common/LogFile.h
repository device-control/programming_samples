#ifndef _LOG_FILE_H_
#define _LOG_FILE_H_

#include <windows.h>
#include <string>
#include <stdio.h>
#include "Log.h"
#include "switchport.h"

class API LogFile : Log
{
private:
	std::FILE* m_pFile;
	std::string m_path;
public:
	// コンストラクタ
	LogFile(const char* pFileName);
	// デストラクタ
	~LogFile();
	bool open();
	bool write(unsigned int level,  const char* format, ...);
	bool close();
};

#endif	// _LOG_FILE_H_
