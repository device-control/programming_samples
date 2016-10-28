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
public:
	// �R���X�g���N�^
	Crypto(const std::string& password);
	// �f�X�g���N�^
	virtual ~Crypto();
	// �Í���
	bool enc(const std::string& in, std::string& enc);
	// ������
	bool dec(const std::string& in, std::string& dec);
};

#endif	// _CRYPTO_H_
