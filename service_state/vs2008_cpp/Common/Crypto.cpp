#include <windows.h>
#include <WinCrypt.h>
#include <string>
#include "Crypto.h"


#define KEYLENGTH_128 (0x0080 * 0x10000) // 128-bit長

///////////////////////////////////////////////////////////////////////////////
Crypto::Crypto(const std::string& password, DWORD max_length/*= 4096*/)
	: m_password(password)
	, m_max_length(max_length)
	, m_pBuffer(NULL)
	, m_hProv(NULL)
	, m_hHash(NULL)
	, m_hKey(NULL)
{
	m_pBuffer = new BYTE[m_max_length];
	if( 0 != Initilize() ){
		// 異常
	}
}

int Crypto::Initilize()
{
	/// http://www.trustss.co.jp/smnEncrypt010.html
	/// CryptoAPIで暗号化・復号の処理をするには、CSP（CryptoGraphy Service Provider）という仕組みを利用
	// 
	if(!CryptAcquireContext(
		&m_hProv, // CSP ハンドル
		NULL, // キーコンテナ名
		MS_ENHANCED_PROV, // CSP名（MS_DEF_PROVでもいいかも)
		PROV_RSA_FULL, // プロバイダタイプ 
		CRYPT_VERIFYCONTEXT // 特定の鍵コンテナをオープンせずにCSPのハンドルを取得(共通鍵暗号のため0を指定してキーコンテナーの存在チェックはしない）
		) ) {
		//printf("ERROR:CryptAcquireContext()\n");
		return 1;
	}
	
	/// パスワードからハッシュ値を計算し、その結果を鍵の種として利用
	// ハッシュ計算
	if(!CryptCreateHash(
		m_hProv, // CSP ハンドル
		CALG_SHA, // ハッシュアルゴリズム
		0, // ハッシュキー
		0, // リザーブ
		&m_hHash // ハッシュオブジェクト
		) ) {
		//printf("ERROR:CryptCreateHash()\n");
		return 2;
	}
	// ハッシュ値の計算
	if(!CryptHashData(
		m_hHash, // ハッシュ ハンドル
		(BYTE*)m_password.c_str(), 
		(DWORD)m_password.size(), 
		0 // フラグ
		) ) {
		//printf("ERROR:CryptHashData()\n");
		return 3;
	}
	
	// 鍵生成
	if(!CryptDeriveKey(
		m_hProv, 
		CALG_RC4, 
		m_hHash, 
		KEYLENGTH_128, 
		&m_hKey
		) ) {
		//printf("ERROR:CryptDeriveKey()\n");
		return 4;
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
Crypto::~Crypto()
{
	if(m_pBuffer){
		delete m_pBuffer;
		m_pBuffer = NULL;
	}
	if(m_hKey){
		CryptDestroyKey(m_hKey);
		m_hKey = NULL;
	}
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
	if( length >= m_max_length ) return false;
	memcpy(m_pBuffer, in.c_str(), length);
	if( !CryptEncrypt(m_hKey, 0, TRUE, 0, m_pBuffer, &length, m_max_length) ){
		//printf("ERROR: CryptEncrypt()\n");
		return false;
	}
	enc = std::string((char*)m_pBuffer, length);
	return true;
}

////////////////////////////////////////////////////////////////////////////////
bool Crypto::dec(const std::string& in, std::string& dec)
{
	DWORD length = in.size();
	if( length >= m_max_length ) return false;
	memcpy(m_pBuffer, in.c_str(), length);

	if(!CryptDecrypt(m_hKey, 0, TRUE, 0, m_pBuffer, &length)) {
		//printf("ERROR: CryptDecrypt()\n");
		return false;
	}
	dec = std::string((char*)m_pBuffer, length);
	return true;
}

