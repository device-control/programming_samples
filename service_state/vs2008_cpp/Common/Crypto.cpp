#include <windows.h>
#include <WinCrypt.h>
#include <string>
#include "Crypto.h"

// TODO: CryptoManager�N���X��V�݂��āA�w�肳�ꂽ�Í���������Crypto�C���X�^���X�𐶐�����
//       Crypto �C���X�^���X�ɂ́A�Ώ́^��Ώ̂Ƃ킸���i���J�^�閧�j�̃G�N�X�|�[�g�A�C���|�[�g���ł���悤�ɂ���
//       Crypto <- CryptoXXX �̂悤�Ɍp������B���p�ґ��� Crypto �N���X�̃C���^�[�t�F�[�X�݂̂ňÍ����^���������s��
//       �悤�ɂ�����

#define KEYLENGTH_128 (0x0080 * 0x10000) // 128-bit��

///////////////////////////////////////////////////////////////////////////////
// Crypto::Crypto(const std::string& password, DWORD max_length/*= 4096*/)
Crypto::Crypto(const std::string& password, unsigned int max_length/*= 4096*/)
	: m_password(password)
	, m_max_length(max_length)
	, m_pBuffer(NULL)
	, m_hProv(NULL)
	, m_hHash(NULL)
	, m_hKey(NULL)
{
	m_pBuffer = new BYTE[m_max_length];
	if( 0 != initilize() ){
		// �ُ�
	}
}

int Crypto::initilize()
{
	/// http://www.trustss.co.jp/smnEncrypt010.html
	/// CryptoAPI�ňÍ����E�����̏���������ɂ́ACSP�iCryptoGraphy Service Provider�j�Ƃ����d�g�݂𗘗p
	// 
	if(!CryptAcquireContext(
		&m_hProv, // CSP �n���h��
		NULL, // �L�[�R���e�i��
		MS_ENHANCED_PROV, // CSP���iMS_DEF_PROV�ł���������)
		PROV_RSA_FULL, // �v���o�C�_�^�C�v 
		CRYPT_VERIFYCONTEXT // ����̌��R���e�i���I�[�v��������CSP�̃n���h�����擾(���ʌ��Í��̂���0���w�肵�ăL�[�R���e�i�[�̑��݃`�F�b�N�͂��Ȃ��j
		) ) {
		//printf("ERROR:CryptAcquireContext()\n");
		return 1;
	}
	
	/// �p�X���[�h����n�b�V���l���v�Z���A���̌��ʂ����̎�Ƃ��ė��p
	// �n�b�V���v�Z
	if(!CryptCreateHash(
		m_hProv, // CSP �n���h��
		CALG_SHA, // �n�b�V���A���S���Y��
		0, // �n�b�V���L�[
		0, // ���U�[�u
		&m_hHash // �n�b�V���I�u�W�F�N�g
		) ) {
		//printf("ERROR:CryptCreateHash()\n");
		return 2;
	}
	// �n�b�V���l�̌v�Z
	if(!CryptHashData(
		m_hHash, // �n�b�V�� �n���h��
		(BYTE*)m_password.c_str(), 
		(DWORD)m_password.size(), 
		0 // �t���O
		) ) {
		//printf("ERROR:CryptHashData()\n");
		return 3;
	}
	
	// ������
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
	unsigned int length = in.size();
	if( length >= m_max_length ) return false;
	memcpy(m_pBuffer, in.c_str(), length);
	if( !CryptEncrypt(m_hKey, 0, TRUE, 0, m_pBuffer, (DWORD *)&length, (DWORD)m_max_length) ){
		//printf("ERROR: CryptEncrypt()\n");
		return false;
	}
	enc = std::string((char*)m_pBuffer, length);
	return true;
}

////////////////////////////////////////////////////////////////////////////////
bool Crypto::dec(const std::string& in, std::string& dec)
{
	unsigned int length = in.size();
	if( length >= m_max_length ) return false;
	memcpy(m_pBuffer, in.c_str(), length);

	if(!CryptDecrypt(m_hKey, 0, TRUE, 0, m_pBuffer, (DWORD *)&length)) {
		//printf("ERROR: CryptDecrypt()\n");
		return false;
	}
	dec = std::string((char*)m_pBuffer, length);
	return true;
}

