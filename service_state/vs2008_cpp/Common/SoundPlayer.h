#ifndef _SOUND_PLAYER_H_
#define _SOUND_PLAYER_H_

#include <windows.h>
#include <string>
#include "switchport.h"

class API SoundPlayer
{
private:
	HWAVEOUT m_hwo;
	WAVEHDR m_wh;

public:
	SoundPlayer();
	~SoundPlayer();

	// ƒCƒxƒ“ƒg–¼
	void play(unsigned char* pWave, int size);
	void stop();
};

#endif	// _SOUND_PLAYER_H_
