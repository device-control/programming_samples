#include "LogFile.h"
#include "ThreadLocker.h"


////////////////////////////////////////////////////////////////////////////////
LogFile::LogFile(const char* pPath)
	: m_pFile(NULL) // closeを設定
	, m_path(pPath)
{
}

////////////////////////////////////////////////////////////////////////////////
LogFile::~LogFile()
{
}

////////////////////////////////////////////////////////////////////////////////
bool LogFile::open()
{
	if( NULL != m_pFile){
		return false; // open済み
	}
	// https://msdn.microsoft.com/ja-jp/library/8f30b0db.aspx
	// a : ファイルの末尾への書き込み用にファイルを開きます (追加モード)。ファイルが存在しない場合は、まず、ファイルを作成します。
	m_pFile = _fsopen(m_path.c_str(), "a", _SH_DENYWR);
	if (NULL != m_pFile) {
		return true;
	}
	return false;
}


////////////////////////////////////////////////////////////////////////////////
// 参考:
//  http://www.02.246.ne.jp/~torutk/cxx/tips/varargs.html
bool LogFile::write(unsigned int level,  const char* format, ...)
{
	ThreadLocker lock(m_locker);
	if( !(m_level & level) ){
		return true; // 出力許可がない
	}
	
    va_list ap;
    va_start(ap, format);
    char* allocatedBuffer;
    //int size = vasprintf(&allocatedBuffer, format, ap); // windowsにはない
	int len = _vscprintf(format, ap);
    allocatedBuffer = new char [len + 1];
	int size = vsprintf_s(allocatedBuffer, len + 1, format, ap);
    va_end(ap);
	size_t numWrote = fprintf(m_pFile, allocatedBuffer);
    free(allocatedBuffer);
	return true;
}

////////////////////////////////////////////////////////////////////////////////
bool LogFile::close()
{
	if( NULL == m_pFile){
		return false; // close済み
	}
	if ( fclose(m_pFile) == 0) {
		m_pFile = NULL;
		return true;
	}
	return false;
}


