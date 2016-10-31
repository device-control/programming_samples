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
	m_date = Date(std::string(date));
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

void LogManager::deleteFiles(int days)
{
	Date past_date = m_date - days;
	std::vector<std::string> list = getFileList();
	for(unsigned int i=0;i<list.size();i++){
		std::string y = std::string(list[i].c_str(),4); 
		std::string m = std::string(list[i].c_str()+4,2);
		std::string d = std::string(list[i].c_str()+6,2);
		Date date(atoi(y.c_str()), atoi(m.c_str()), atoi(d.c_str()));
		if( date > m_date ){
			// �����ُ͈펖�ԂȂ�ŉ������Ȃ�
			continue;
		}
		if( date < past_date ){
			// �w�肳�ꂽ�͈͊O�̉ߋ��͍폜
			std::string file = m_path + "\\" + list[i];
			DeleteFileA(file.c_str());
		}
	}
}
