#include "stdafx.h"
#include "SoundPlayer.h"
#include "MainFrm.h"
#include "WeeklyPlannerView.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm")

CSoundPlayer::CSoundPlayer()
	: m_nSoundIndex(0)
{
	//default로 들어있는 Sound
	m_strSoundPath[0] = _T("res\\MFC_SOUND\\Barroom_Ballet_Silent_Film_Light.wav");
	m_strSoundPath[1] = _T("res\\MFC_SOUND\\Greedy.wav");
	m_strSoundPath[2] = _T("res\\MFC_SOUND\\Merry_Go_Slower.wav");
	m_strSoundPath[3] = _T("res\\MFC_SOUND\\Old_MacDonald.wav");
	m_strSoundPath[4] = _T("res\\MFC_SOUND\\Ponies_and_Balloons");
	m_strSoundPath[5] = _T("res\\MFC_SOUND\\Rainy_Day_Games");
	m_strSoundPath[6] = _T("res\\MFC_SOUND\\Summer_Smile.wav");
	m_strSoundPath[7] = _T("res\\MFC_SOUND\\Toy_Piano.wav");
	m_strSoundPath[8] = _T("res\\MFC_SOUND\\Washington_Post.wav");

	m_strSoundName[0] = _T("Barroom_Ballet_Silent_Film_Light");
	m_strSoundName[1] = _T("Greedy");
	m_strSoundName[2] = _T("Merry_Go_Slower");
	m_strSoundName[3] = _T("Old_MacDonald");
	m_strSoundName[4] = _T("Ponies_and_Balloons");
	m_strSoundName[5] = _T("Rainy_Day_Games");
	m_strSoundName[6] = _T("Summer_Smile");
	m_strSoundName[7] = _T("Toy_Piano");
	m_strSoundName[8] = _T("Washington_Post");

	m_nSoundIndex = 9;
}


CSoundPlayer::~CSoundPlayer()
{
}


void CSoundPlayer::SoundPlay(int soundIndex)
{
	PlaySound(m_strSoundPath[soundIndex], AfxGetInstanceHandle(), SND_ASYNC);

}


void CSoundPlayer::SoundStop()
{
	sndPlaySound(NULL, SND_ASYNC);
}
