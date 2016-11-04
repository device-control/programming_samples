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
	// コンストラクタ
	// Crypto(const std::string& password, DWORD max_length = 4096);
	Crypto(const std::string& password, unsigned int max_length = 4096);
	// デストラクタ
	virtual ~Crypto();
	// 暗号化
	bool enc(const std::string& in, std::string& enc);
	// 複合化
	bool dec(const std::string& in, std::string& dec);
private:
	int initilize();
};

#endif	// _CRYPTO_H_
