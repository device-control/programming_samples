#include <windows.h>
#include <WinCrypt.h>
#include <string>
#include "Crypto.h"


#define KEYLENGTH_128 (0x0080 * 0x10000) // 128-bit��

///////////////////////////////////////////////////////////////////////////////
Crypto::Crypto(const std::string& password)
	: m_password(password)
	, m_hProv(NULL)
	, m_hHash(NULL)
	, m_hKey(NULL)
{
	if(!CryptAcquireContext(
		&m_hProv, // CSP �n���h��
		NULL, // �L�[�R���e�i��
		MS_ENHANCED_PROV, // CSP��
		PROV_RSA_FULL, // �v���o�C�_�^�C�v 
		CRYPT_VERIFYCONTEXT // ����̌��R���e�i���I�[�v��������CSP�̃n���h�����擾
		) ) {
		//printf("ERROR:CryptAcquireContext()\n");
	}
	
	//	�n�b�V���v�Z
	if(!CryptCreateHash(
		m_hProv, // CSP �n���h��
		CALG_SHA, // �n�b�V���A���S���Y��
		0, // �n�b�V���L�[
		0, // ���U�[�u
		&m_hHash // �n�b�V���I�u�W�F�N�g
		) ) {
		//printf("ERROR:CryptCreateHash()\n");
	}

	//	�n�b�V���l�̌v�Z
	if(!CryptHashData(
		m_hHash, // �n�b�V�� �n���h��
		(BYTE*)m_password.c_str(), 
		(DWORD)m_password.size() - 1, 
		0 // �t���O
		) ) {
		//printf("ERROR:CryptHashData()\n");
	}
	
	//	������
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
	length += 1; // �{�P���闝�R�͕s���B�T���v���̂܂܂Ƃ��Ă���
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

