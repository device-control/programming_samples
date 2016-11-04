#include "LogFile.h"
#include "ThreadLocker.h"


////////////////////////////////////////////////////////////////////////////////
LogFile::LogFile(const char* pPath)
	: m_pFile(NULL) // close��ݒ�
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
		return false; // open�ς�
	}
	// https://msdn.microsoft.com/ja-jp/library/8f30b0db.aspx
	// a : �t�@�C���̖����ւ̏������ݗp�Ƀt�@�C�����J���܂� (�ǉ����[�h)�B�t�@�C�������݂��Ȃ��ꍇ�́A�܂��A�t�@�C�����쐬���܂��B
	m_pFile = _fsopen(m_path.c_str(), "ab", _SH_DENYWR);
	if (NULL != m_pFile) {
		return true;
	}
	return false;
}


////////////////////////////////////////////////////////////////////////////////
// �Q�l:
//  http://www.02.246.ne.jp/~torutk/cxx/tips/varargs.html
bool LogFile::write(unsigned int level,  const char* format, ...)
{
	ThreadLocker lock(m_locker);
	if( !(m_level & level) ){
		return true; // �o�͋����Ȃ�
	}
	
    va_list ap;
    va_start(ap, format);
    char* allocatedBuffer;
    //int size = vasprintf(&allocatedBuffer, format, ap); // windows�ɂ͂Ȃ�
	int len = _vscprintf(format, ap);
    allocatedBuffer = new char [len + 1];
	int size = vsprintf_s(allocatedBuffer, len + 1, format, ap);
    va_end(ap);
	if( m_pCrypto ){
		// �Í������ݒ肳�ꂽ�ꍇ
		std::string in(allocatedBuffer);
		std::string out;
		m_pCrypto->enc(in,out);
		int size = out.size();
		fwrite(&size, sizeof(int), 1, m_pFile);
		fwrite(out.c_str(), sizeof(char), out.size(), m_pFile);
	}
	else{
		fprintf(m_pFile, allocatedBuffer);
	}
    free(allocatedBuffer);
	return true;
}

////////////////////////////////////////////////////////////////////////////////
bool LogFile::close()
{
	if( NULL == m_pFile){
		return false; // close�ς�
	}
	if ( fclose(m_pFile) == 0) {
		m_pFile = NULL;
		return true;
	}
	return false;
}


