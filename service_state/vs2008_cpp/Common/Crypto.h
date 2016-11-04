#ifndef _CRYPTO_H_
#define _CRYPTO_H_

#include <windows.h>
#include <WinCrypt.h>
#include <string>
#include "switchport.h"

class API Crypto {

private:
	HCRYPTPROV	m_hProv;
	HCRYPTHASH	m_hHash;
	HCRYPTKEY	m_hKey;
	std::string m_password;
	unsigned int m_max_length;
	BYTE* m_pBuffer;
public:
	// �R���X�g���N�^
	// Crypto(const std::string& password, DWORD max_length = 4096);
	Crypto(const std::string& password, unsigned int max_length = 4096);
	// �f�X�g���N�^
	virtual ~Crypto();
	// �Í���
	bool enc(const std::string& in, std::string& enc);
	// ������
	bool dec(const std::string& in, std::string& dec);
private:
	int initilize();
};

#endif	// _CRYPTO_H_
