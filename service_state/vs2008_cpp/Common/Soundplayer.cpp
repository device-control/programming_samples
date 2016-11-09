#include <windows.h>

#include "SoundPlayer.h"

////////////////////////////////////////////////////////////////////////////////
SoundPlayer::SoundPlayer()
	: m_hwo(0)
{
}

SoundPlayer::~SoundPlayer()
{
	stop();
}

////////////////////////////////////////////////////////////////////////////////
void SoundPlayer::play(unsigned char* pWave, int size)
{
	if( NULL == pWave || size < 20 ){
		// パラメータ異常
		// FIXME: 詳細はフォーマットチェックが本来は必要。
		//        今回は正常なwaveファイル(確認済み)しか鳴らさない。
		return;
	}
	stop();// 前回再生分を停止

	WAVEFORMATEX* pwf = (WAVEFORMATEX *)(pWave + 20);
	waveOutOpen( &m_hwo, WAVE_MAPPER, ( LPWAVEFORMATEX )( pWave + 20 ), NULL, NULL, CALLBACK_NULL );
	if( m_hwo ) {
		int *pi = ( int * )( pWave + 16 );
		memset( &m_wh, 0, sizeof( WAVEHDR ) );
		m_wh.lpData = ( LPSTR )( pWave + 16 + *pi + 16 );
		m_wh.dwBufferLength = *( int * )( pWave + 16 + *pi + 8 );
		m_wh.dwFlags = WHDR_BEGINLOOP | WHDR_ENDLOOP;
		m_wh.dwLoops = 1;
		if( MMSYSERR_NOERROR == waveOutPrepareHeader( m_hwo, &m_wh, sizeof( WAVEHDR ) ) ) {
			waveOutWrite( m_hwo, &m_wh, sizeof( WAVEHDR ) );
			Sleep(100);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
void SoundPlayer::stop()
{
	if( m_hwo ){
		waveOutReset(m_hwo);
		waveOutUnprepareHeader(m_hwo,&m_wh,sizeof(WAVEHDR));
		waveOutClose(m_hwo);
	}
}
