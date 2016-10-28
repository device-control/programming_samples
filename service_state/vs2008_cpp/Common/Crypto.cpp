#include <windows.h>
#include <WinCrypt.h>
#include <string>
#include "Crypto.h"


#define KEYLENGTH_128 (0x0080 * 0x10000) // 128-bit長

///////////////////////////////////////////////////////////////////////////////
Crypto::Crypto(const std::string& password)
	: m_password(password)
	, m_hProv(NULL)
	, m_hHash(NULL)
	, m_hKey(NULL)
{
	if(!CryptAcquireContext(
		&m_hProv, // CSP ハンドル
		NULL, // キーコンテナ名
		MS_ENHANCED_PROV, // CSP名
		PROV_RSA_FULL, // プロバイダタイプ 
		CRYPT_VERIFYCONTEXT // 特定の鍵コンテナをオープンせずにCSPのハンドルを取得
		) ) {
		//printf("ERROR:CryptAcquireContext()\n");
	}
	
	//	ハッシュ計算
	if(!CryptCreateHash(
		m_hProv, // CSP ハンドル
		CALG_SHA, // ハッシュアルゴリズム
		0, // ハッシュキー
		0, // リザーブ
		&m_hHash // ハッシュオブジェクト
		) ) {
		//printf("ERROR:CryptCreateHash()\n");
	}

	//	ハッシュ値の計算
	if(!CryptHashData(
		m_hHash, // ハッシュ ハンドル
		(BYTE*)m_password.c_str(), 
		(DWORD)m_password.size() - 1, 
		0 // フラグ
		) ) {
		//printf("ERROR:CryptHashData()\n");
	}
	
	//	鍵生成
	if(!CryptDeriveKey(
		m_hProv, 
		CALG_RC4, 
		m_hHash, 
		KEYLENGTH_128, 
		&m_hKey
		) ) {
		//printf("ERROR:CryptDeriveKey()\n");
	}
}

////////////////////////////////////////////////////////////////////////////////
Crypto::~Crypto()
{
	if(m_hHash){
		CryptDestroyHash(m_hHash);
		m_hHash = NULL;
	}
	if(m_hProv){
		CryptReleaseContext(m_hProv, 0);
		m_hProv = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////
bool Crypto::enc(const std::string& in, std::string& enc)
{
	DWORD length = in.size();
	if( length >= 2048 ) return false;
	BYTE buffer[2048+1];
	memcpy(buffer, in.c_str(), length);
	length += 1; // ＋１する理由は不明。サンプルのままとしておく
	if( !CryptEncrypt(m_hKey, 0, TRUE, 0, buffer, &length, 2048+1) ){
		//printf("ERROR: CryptEncrypt()\n");
		return false;
	}
	enc = std::string((char*)buffer, length);
	return true;
}

////////////////////////////////////////////////////////////////////////////////
bool Crypto::dec(const std::string& in, std::string& dec)
{
	DWORD length = in.size();
	if( length >= 2048 ) return false;
	BYTE buffer[2048+1];
	memcpy(buffer, in.c_str(), length);

	if(!CryptDecrypt(m_hKey, 0, TRUE, 0, buffer, &length)) {
		//printf("ERROR: CryptDecrypt()\n");
		return false;
	}
	dec = std::string((char*)buffer, length - 1);
	return true;
}

