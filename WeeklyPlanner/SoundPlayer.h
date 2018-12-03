#pragma once
class CSoundPlayer
{
public:
	CSoundPlayer();
	~CSoundPlayer();
	int m_nSoundIndex;
	CString m_strSoundPath[50];
	void SoundPlay(int soundIndex);
	void SoundStop();
	CString m_strSoundName[50];
};

