#ifndef _LOG_FILE_H_
#define _LOG_FILE_H_

#include <windows.h>
#include <string>
#include <stdio.h>
#include "Log.h"
#include "switchport.h"

class API LogFile : public Log
{
private:
	std::FILE* m_pFile;
	std::string m_path;
public:
	// �R���X�g���N�^
	LogFile(const char* pFileName);
	// �f�X�g���N�^
	~LogFile();
	bool open();
	bool write(unsigned int level,  const char* format, ...);
	bool close();
};

#endif	// _LOG_FILE_H_
