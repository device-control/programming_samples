#ifndef _LOG_MANAGER_H_
#define _LOG_MANAGER_H_

#include <windows.h>
#include <string>
#include <vector>
#include "Date.h"
#include "switchport.h"

class API LogManager {

private:
    std::string m_path; // 出力パス
    std::string m_ext; // 拡張子
	Date m_date; // ログ日付
public:
	// コンストラクタ
	LogManager(const char* date, const char *path);
	LogManager(const char *path);
	// デストラクタ
	virtual ~LogManager();
	std::string getFileName();
	std::vector<std::string> getFileList();
	int dayRetentionOf(int days);
	static bool createDirectory(const char* pPath);
	static bool deleteDirectory(const char* pPath);
	static std::vector<std::string> split(const std::string &str, char sep='\\');
protected:
	// コンストラクタ
	LogManager();

};

#endif	// _LOG_MANAGER_H_
