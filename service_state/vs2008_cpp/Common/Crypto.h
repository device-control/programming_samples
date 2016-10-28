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
	// コンストラクタ
	Crypto(const std::string& password);
	// デストラクタ
	virtual ~Crypto();
	// 暗号化
	bool enc(const std::string& in, std::string& enc);
	// 複合化
	bool dec(const std::string& in, std::string& dec);
};

#endif	// _CRYPTO_H_
