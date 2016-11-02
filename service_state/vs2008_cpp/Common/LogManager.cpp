#include <direct.h>
#include <sstream>
// #include <tchar.h> //_T()�}�N�����p�@TODO: xxxW(), xxxA() �ӎ����邱�ƂȂ��Ăяo���������ǈӖ��s��
#include "LogManager.h"

///////////////////////////////////////////////////////////////////////////////
LogManager::LogManager()
{
}

///////////////////////////////////////////////////////////////////////////////
// ���t�w�� "YYYY/MM/DD"
LogManager::LogManager(const char* date, const char *path)
	: m_path(path)
	, m_ext("log")
{
	m_date = Date(date);
}

///////////////////////////////////////////////////////////////////////////////
// ���ݓ��t
LogManager::LogManager(const char *path)
	: m_path(path)
	, m_ext("log")
{
	m_date = Date::getToday();
}

////////////////////////////////////////////////////////////////////////////////
LogManager::~LogManager()
{
}

////////////////////////////////////////////////////////////////////////////////
std::string LogManager::getFileName()
{
	char buffer[128];
	sprintf_s(buffer, "%s\\%04d%02d%02d.%s", m_path.c_str(), m_date.m_year, m_date.m_month, m_date.m_day, m_ext.c_str());
	return std::string(buffer);
}

////////////////////////////////////////////////////////////////////////////////
// unicode �p(A�t�֐��g�p)
// �Q�l�T�C�g
// http://qwertyfk.blog16.fc2.com/blog-entry-118.html
// http://qiita.com/tkymx/items/f9190c16be84d4a48f8a
std::vector<std::string> LogManager::getFileList()
{
	HANDLE hFind;
	WIN32_FIND_DATAA fd;//defined at Windwos.h
	std::vector<std::string> list;

    //�g���q�̐ݒ�
    std::string search_name = m_path + "\\*." + m_ext;
	hFind = FindFirstFileA( search_name.c_str() , &fd);

    if (hFind == INVALID_HANDLE_VALUE) {
        // file not found
		return list;
    }

    do {
		// �t�H���_�͏���
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
		// �B���t�@�C���͏���
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) continue;
		list.push_back(fd.cFileName);
    } while (FindNextFileA(hFind, &fd));
    FindClose(hFind);

    return list;
}

// ���O��ێ��������
int LogManager::dayRetentionOf(int days)
{
	Date past_date = m_date - days;
	std::vector<std::string> list = getFileList();
	int deleteFileCount=0;
	for(unsigned int i=0;i<list.size();i++){
		std::string y = std::string(list[i].c_str(),4); 
		std::string m = std::string(list[i].c_str()+4,2);
		std::string d = std::string(list[i].c_str()+6,2);
		Date date(atoi(y.c_str()), atoi(m.c_str()), atoi(d.c_str()));
		//if( date > m_date ){
		//	// �������͂��肦�Ȃ��̂ŉ������Ȃ�
		//	continue;
		//}
		if( date < past_date ){
			// �w�肳�ꂽ�͈͊O�̉ߋ��͍폜
			std::string file = m_path + "\\" + list[i];
			DeleteFileA(file.c_str());
			deleteFileCount++;
		}
	}
	return deleteFileCount;
}

/*static*/
std::vector<std::string> LogManager::split(const std::string &str, char sep/*='\\'*/)
{
    std::vector<std::string> v;
    std::stringstream ss(str);
    std::string buffer;
    while( std::getline(ss, buffer, sep) ) {
        v.push_back(buffer);
    }
    return v;
}

/*static*/
bool LogManager::createDirectory(const char* pPath)
{
	std::vector<std::string> list = LogManager::split(std::string(pPath));
	std::string path = ".";
	for(unsigned int i=0;i<list.size();i++){
		path += "\\" + list[i];
		_mkdir(path.c_str());
	}
	return true;
}

/*static*/
bool LogManager::deleteDirectory(const char* pPath)
{
	HANDLE hFind;
	WIN32_FIND_DATAA fd;//defined at Windwos.h

	//�g���q�̐ݒ�
	std::string path = pPath;
	std::string search_name = path;
	search_name += "\\*.*";
	hFind = FindFirstFileA( search_name.c_str() , &fd);
	if (hFind == INVALID_HANDLE_VALUE) {
		return RemoveDirectoryA(pPath) ? true: false;
	}
	do{
		if( strcmp(fd.cFileName,".") == 0 ||
			strcmp(fd.cFileName,"..") == 0 ){
			continue;
		}
		std::string new_path = path + "\\" + fd.cFileName;
		// �t�H���_
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			
			LogManager::deleteDirectory(new_path.c_str());
			continue;
		}
		DeleteFileA(new_path.c_str());
	} while (FindNextFileA(hFind, &fd));
	FindClose(hFind);
	return RemoveDirectoryA(pPath) ? true: false;
}
