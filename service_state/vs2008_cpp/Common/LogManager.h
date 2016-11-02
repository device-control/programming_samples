#ifndef _LOG_MANAGER_H_
#define _LOG_MANAGER_H_

#include <windows.h>
#include <string>
#include <vector>
#include "Date.h"
#include "switchport.h"

class API LogManager {

private:
    std::string m_path; // �o�̓p�X
    std::string m_ext; // �g���q
	Date m_date; // ���O���t
public:
	// �R���X�g���N�^
	LogManager(const char* date, const char *path);
	LogManager(const char *path);
	// �f�X�g���N�^
	virtual ~LogManager();
	std::string getFileName();
	std::vector<std::string> getFileList();
	int dayRetentionOf(int days);
	static bool createDirectory(const char* pPath);
	static bool deleteDirectory(const char* pPath);
	static std::vector<std::string> split(const std::string &str, char sep='\\');
protected:
	// �R���X�g���N�^
	LogManager();

};

#endif	// _LOG_MANAGER_H_
